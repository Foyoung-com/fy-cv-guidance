#include "conf_serializer.hpp"
#include "fy_log.hpp"

#include <opencv2/opencv.hpp>
#include <DJI_guidance.h>

#include <algorithm>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace fc = fy::checker;

using fy::Log;

#define TAG "CheckerBoard"
#define TWIDTH 320
#define TEIGHT 240 //Guidance image is always 320x240 pixels, 8 bit grayscale
#define TISIZE (TWIDTH * TEIGHT)

#define RETURN_IF_ERR(err_code)                                                                                  \
    {                                                                                                            \
        if (err_code)                                                                                            \
        {                                                                                                        \
            Log::LogError(TAG, "guidance error", "error code:%d in file %s %d\n", err_code, __FILE__, __LINE__); \
            return err_code;                                                                                     \
        }                                                                                                        \
    }
#define RELEASE_IF_ERR(err_code)                                                                                 \
    {                                                                                                            \
        if (err_code)                                                                                            \
        {                                                                                                        \
            release_transfer();                                                                                  \
            Log::LogError(TAG, "guidance error", "error code:%d in file %s %d\n", err_code, __FILE__, __LINE__); \
            return err_code;                                                                                     \
        }                                                                                                        \
    }

std::mutex data_mutex;
std::condition_variable data_cond_var;
cv::Mat left_image(TEIGHT, TWIDTH, CV_8U);
cv::Mat process_image(TEIGHT, TWIDTH, CV_8U);
cv::Mat *p1 = &left_image;
cv::Mat *p2 = &process_image;
cv::VideoCapture capture;

int vbus_index;
int left_image_ready = false;

int guidance_event_callback(int data_type, int data_len, char *data)
{
    if (e_guidance_event::e_image == data_type && data)
    {
        {
            std::lock_guard<std::mutex> __locker(data_mutex);

            image_data d;
            memcpy((char *)&d, data, sizeof(d));
            memcpy(p1->data, d.m_greyscale_image_left[vbus_index], TISIZE);
            left_image_ready = true;
        }
        data_cond_var.notify_one();
    }
}

int initialize(const fc::ConfSerializer &conf)
{
    reset_config();

    int err_code = init_transfer();
    RETURN_IF_ERR(err_code);

    vbus_index = conf.getGuidanceVBusIndex();
    if (vbus_index < e_vbus5 || vbus_index > e_vbus4)
    {
        Log::LogError(TAG, "config file error", "vbus index(%d) is out of range[%d-%d]", vbus_index, e_vbus5, e_vbus4);
        return -1;
    }

    e_vbus_index vi = static_cast<e_vbus_index>(vbus_index);
    err_code = select_greyscale_image(vi, true);
    RELEASE_IF_ERR(err_code);

    err_code = set_sdk_event_handler(guidance_event_callback);
    RELEASE_IF_ERR(err_code);

    err_code = start_transfer();
    RELEASE_IF_ERR(err_code);

    return 0;
}

int finalize()
{

    RELEASE_IF_ERR(stop_transfer());

    std::this_thread::sleep_for(std::chrono::duration<int>(5));

    RETURN_IF_ERR(release_transfer());

    return 0;
}

cv::Mat next_guidance()
{

    std::unique_lock<std::mutex> __locker(data_mutex);
    data_cond_var.wait(__locker, [] { return left_image_ready; });
    std::swap(p1, p2);
    left_image_ready = false;
    return *p2;
}

cv::Mat next_capture()
{
    cv::Mat frame;
    static cv::Mat grey;
    capture >> frame;
    cv::cvtColor(frame, grey, CV_BGR2GRAY);
    cv::resize(grey, process_image, cv::Size(TWIDTH, TEIGHT));
    return process_image;
}

cv::Mat next(fc::SSource::Enum s)
{
    if (s == fc::SSource::GUIDANCE)
        return next_guidance();
    else
        return next_capture();
}

std::string timestring()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[50];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%I-%M-%S_%d-%m-%Y", timeinfo);
    return buffer;
}

int main(int argc, const char **argv)
{
    std::string config;
    if (argc == 2)
        config = argv[1];

    fc::ConfSerializer conf(config);

    fc::SSource::Enum source = conf.getSource();
    switch (source)
    {
    case fc::SSource::GUIDANCE:
        if (initialize(conf))
            return -1;
        break;
    case fc::SSource::FILE:
    {
        std::string sourceVideoFile = conf.getSourceFile();
        if (!capture.open(sourceVideoFile))
        {
            Log::LogError(TAG, "config file error", "could not open source video file: %s\n", sourceVideoFile.c_str());
            return -2;
        }
    }
    break;
    case fc::SSource::CAMERA:
    {
        int cameraIndex = conf.getCameraIndex();
        if (!capture.open(cameraIndex))
        {
            Log::LogError(TAG, "config file error", "could not open camera at index %d \n", cameraIndex);
            return -3;
        }
    }
    break;
    default:
        Log::LogError(TAG, "config file error", "source (%d) is out of range[%d-%d]", conf.getSource(), fc::SSource::GUIDANCE, fc::SSource::CAMERA);
        return -4;
    }

    int hc, wc;
    conf.getCheckerPatterns(hc, wc);
    cv::Size patternsize(hc - 1, wc - 1);
    std::vector<cv::Point2f> corners;

    //open video writer
    cv::VideoWriter writer;
    if (conf.isEnableVideo())
    {
        auto videoFileName = timestring();
        videoFileName = "_" + videoFileName + ".avi";
        auto confFileName = conf.getFileName();
        auto dotPos = confFileName.rfind('.');
        videoFileName = confFileName.replace(dotPos, confFileName.length() - dotPos, videoFileName);
        writer.open(videoFileName, CV_FOURCC('M', 'J', 'P', 'G'), 10, cv::Size(TWIDTH, TEIGHT), false);
        if (!writer.isOpened())
        {
            Log::LogError(TAG, "runtime error", "Could not open the output video file: %s\n", videoFileName.c_str());
            return -5;
        }
    }

    //open image window
    cv::namedWindow(TAG, 0);

    for (;;)
    {
        cv::Mat frame = next(source);

        bool patternfound = cv::findChessboardCorners(frame, patternsize, corners);
        if (patternfound)
            cv::cornerSubPix(frame, corners, cv::Size(3, 3), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

        cv::drawChessboardCorners(frame, patternsize, cv::Mat(corners), patternfound);

        //display video frame
        cv::imshow(TAG, frame);

        //write video frame
        if (conf.isEnableVideo())
            writer << frame;

        char c = (char)cv::waitKey(10);
        if (c == 27 || c == 'q')
            break;
    }

    if (source == fc::SSource::GUIDANCE)
        if (!finalize())
            return -6;

    return 0;
}
