#include <string>

namespace fy
{
namespace checker
{

struct SSource
{
  enum Enum
  {
    GUIDANCE, //0
    FILE,     //1
    CAMERA,   //2
  };
};

class ConfSerializer
{

public:
  ConfSerializer(const std::string &fileName = "");

  virtual ~ConfSerializer();

  void read();

  void write();

  void enableVideo(bool b);

  bool isEnableVideo() const;

  void setSource(SSource::Enum source);

  SSource::Enum getSource() const;

  void setGuidanceVBusIndex(int index);

  int getGuidanceVBusIndex() const;

  void setSourceFile(const std::string &sourceFile);

  std::string getSourceFile() const;

  void setCameraIndex(int i = 0);

  int getCameraIndex() const;

  void setCheckerPattern(int hc, int wc);

  void getCheckerPatterns(int &hc, int &wc) const;

  std::string getFileName() const { return mFile; }

private:
  std::string mFile;
  std::string mSourceFile;
  SSource::Enum mSource;
  int mHCorners;
  int mWCorners;
  int mVBusIndex;
  int mCameraIndex;
  bool mEnableVideo;
};
}
}
