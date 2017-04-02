#include "conf_serializer.hpp"
#include "opencv2/opencv.hpp"

#include <unordered_map>
#include <memory>

namespace fc = fy::checker;

namespace fy
{
namespace checker
{

#define CONFIG_FILE "checker_conf"
#define CONFIG_VBUD_INDEX 0
#define CONFIG_HORIZ_CORNERS 4
#define CONFIG_COLUM_CORNERS 4

namespace details
{

std::unordered_map<std::string, std::shared_ptr<cv::FileStorage>> smaps;
static int next = 0;

cv::FileStorage &getMyStorage(const std::string &fileName)
{
    return *smaps[fileName];
}

template <typename T>
void readOrSet(cv::FileStorage &storage, const std::string &key, T &v, const T &default_v)
{
    auto fn = storage[key];
    if (fn.isNone())
    {
        v = default_v;
    }
    else
    {
        fn >> v;
    }
}

template <typename T>
void writeConfig(cv::FileStorage &storage, const std::string &key, const T &v)
{
    storage << key << v;
}
}

ConfSerializer::ConfSerializer(const std::string &fileName)
{
    mFile = fileName;
    if (mFile.empty())
    {
        using details::next;
        mFile = CONFIG_FILE + std::to_string(next++) + ".xml";
    }
    auto search = details::smaps.find(mFile);
    if (search == details::smaps.end())
    {
        try
        {
            details::smaps[mFile] = std::make_shared<cv::FileStorage>(mFile, cv::FileStorage::READ);
            read();
            details::smaps[mFile] = std::make_shared<cv::FileStorage>(mFile, cv::FileStorage::WRITE);
        }
        catch (...)
        {
            //error
        }
    }
}

ConfSerializer::~ConfSerializer()
{
    write();
}

void ConfSerializer::read()
{
    cv::FileStorage &fs = details::getMyStorage(mFile);
    using details::readOrSet;
    readOrSet(fs, "enable_video", mEnableVideo, true);
    int _source;
    readOrSet(fs, "source", _source, 1);
    mSource = static_cast<SSource::Enum>(_source);
    readOrSet(fs, "source_file", mSourceFile, std::string(""));
    readOrSet(fs, "camera_index", mCameraIndex, 0);
    readOrSet(fs, "vbus_index", mVBusIndex, CONFIG_VBUD_INDEX);
    readOrSet(fs, "horiz_corners", mHCorners, CONFIG_HORIZ_CORNERS);
    readOrSet(fs, "colum_corners", mWCorners, CONFIG_COLUM_CORNERS);
}

void ConfSerializer::write()
{
    cv::FileStorage &fs = details::getMyStorage(mFile);
    using details::writeConfig;
    writeConfig(fs, "enable_video", mEnableVideo);
    writeConfig(fs, "source", (int)mSource);
    writeConfig(fs, "source_file", mSourceFile);
    writeConfig(fs, "camera_index", mCameraIndex);
    writeConfig(fs, "vbus_index", mVBusIndex);
    writeConfig(fs, "horiz_corners", mHCorners);
    writeConfig(fs, "colum_corners", mWCorners);
}

void ConfSerializer::enableVideo(bool b)
{
    mEnableVideo = b;
}

bool ConfSerializer::isEnableVideo() const
{
    return mEnableVideo;
}

void ConfSerializer::setSource(SSource::Enum source)
{
    mSource = source;
}

SSource::Enum ConfSerializer::getSource() const
{
    return mSource;
}

void ConfSerializer::setGuidanceVBusIndex(int index)
{
    mVBusIndex = index;
}

int ConfSerializer::getGuidanceVBusIndex() const
{
    return mVBusIndex;
}

void ConfSerializer::setSourceFile(const std::string &sourceFile)
{
    mSourceFile = sourceFile;
}

std::string ConfSerializer::getSourceFile() const
{
    return mSourceFile;
}

void ConfSerializer::setCameraIndex(int i)
{
    mCameraIndex = i;
}

int ConfSerializer::getCameraIndex() const
{
    return mCameraIndex;
}

void ConfSerializer::setCheckerPattern(int hc, int wc)
{
    mHCorners = hc;
    mWCorners = wc;
}

void ConfSerializer::getCheckerPatterns(int &hc, int &wc) const
{
    hc = mHCorners;
    wc = mWCorners;
}

#undef CONFIG_FILE
}
}