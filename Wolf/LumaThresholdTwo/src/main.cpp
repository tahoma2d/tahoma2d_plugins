#define TNZU_DEFINE_INTERFACE
#include <toonz_utility.hpp>
#include <opencv2/imgproc/imgproc_c.h>

class MyFx : public tnzu::Fx {
 public:
  //
  // PORT
  //
  enum {
    PORT_INPUT,
    PORT_COUNT,
  };

  int port_count() const override { return PORT_COUNT; }

  char const* port_name(int i) const override {
    static std::array<char const*, PORT_COUNT> names = {
        "Input",
    };
    return names[i];
  }

  //
  // PARAM GROUP
  //
  enum {
    PARAM_GROUP_DEFAULT,
    PARAM_GROUP_COUNT,
  };

  int param_group_count() const override { return PARAM_GROUP_COUNT; }

  char const* param_group_name(int i) const override {
    static std::array<char const*, PARAM_GROUP_COUNT> names = {
        "Default",
    };
    return names[i];
  }

  //
  // PARAM
  //
  enum {
    PARAM_THRESHOLD_MIN,
	PARAM_THRESHOLD_MAX,
	PARAM_INVERT,
	PARAM_KEEPALPHA,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Threshold_Min", PARAM_GROUP_DEFAULT, 0, 0, 255},
		ParamPrototype{"Threshold_Max", PARAM_GROUP_DEFAULT, 255, 0, 255},
		ParamPrototype{"Invert", PARAM_GROUP_DEFAULT, 0, 0, 1},
		ParamPrototype{"Keep_Alpha", PARAM_GROUP_DEFAULT, 0, 0, 1},
    };
    return &params[i];
  }

 public:
  int compute(Config const& config, Params const& params, Args const& args,
		cv::Mat& retimg) override try {
    DEBUG_PRINT(__FUNCTION__);

	if (args.invalid(PORT_INPUT)) {
		return 0;
	}

	const cv::Mat& InImg = args.get(PORT_INPUT);

	const double Threshold_Min = params.get<double>(PARAM_THRESHOLD_MIN);
	const double Threshold_Max = params.get<double>(PARAM_THRESHOLD_MAX);
	const double Invert = params.get<double>(PARAM_INVERT);
	const double KeepAlpha = params.get<double>(PARAM_KEEPALPHA); 

	cv::Mat grayMat;
	cv::cvtColor(InImg, grayMat, CV_BGR2GRAY);
	

	for (int i = 0; i < grayMat.rows; ++i) 
	{
		for (int j = 0; j < grayMat.cols; ++j)
		{

			int Intensity = (int)grayMat.at<uchar>(i, j);

			int MinVal = Invert ? 0 : 255;
			int MaxVal = Invert ? 255 : 0;

			grayMat.at<uchar>(i, j) = Intensity >= Threshold_Min && Intensity <= Threshold_Max ? MinVal : MaxVal;

		}
	}

	cv::cvtColor(grayMat, retimg, CV_GRAY2BGRA);

	//Copy alpha from InImg to retimg
	if (KeepAlpha > 0.999f)
	{
		for (int i = 0; i < retimg.rows; ++i) 
		{
			for (int j = 0; j < retimg.cols; ++j) 
			{

				const cv::Vec4b& InPixel = InImg.at<cv::Vec4b>(i, j);

				cv::Vec4b& RetPixel = retimg.at<cv::Vec4b>(i, j);
				RetPixel[3] = InPixel[3];
			}
		}
	}



    return 0;
  } catch (cv::Exception const& e) {
    DEBUG_PRINT(e.what());
  }
};


namespace tnzu {
PluginInfo const* plugin_info() {
  static PluginInfo const info(TNZU_PP_STR(PLUGIN_NAME),    // name
                               TNZU_PP_STR(PLUGIN_VENDOR),  // vendor
                               "",                          // note
                               "");     // helpurl
  return &info;
}

Fx* make_fx() { return new MyFx(); }
}
