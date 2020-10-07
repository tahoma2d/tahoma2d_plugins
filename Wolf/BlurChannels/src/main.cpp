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
    PARAM_BLUR_R,
    PARAM_BLUR_G,
    PARAM_BLUR_B,
    //PARAM_BLUR_A,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Blur_Amount_R", PARAM_GROUP_DEFAULT, 0, 0, 50},
        ParamPrototype{"Blur_Amount_G", PARAM_GROUP_DEFAULT, 0, 0, 50},
        ParamPrototype{"Blur_Amount_B", PARAM_GROUP_DEFAULT, 0, 0, 50},
        //ParamPrototype{"Blur_Amount_A", PARAM_GROUP_DEFAULT, 0, 0, 50},
    };
    return &params[i];
  }

  int enlarge(Config const& config, Params const& params,
	  cv::Rect2d& retrc) override {
	  DEBUG_PRINT(__FUNCTION__);

	  int const BlurR = params.get<int>(PARAM_BLUR_R);
	  int const BlurG = params.get<int>(PARAM_BLUR_G);
	  int const BlurB = params.get<int>(PARAM_BLUR_B);
	  //int const BlurA = params.get<int>(PARAM_BLUR_A);

	  //int BlurMax = std::max({ BlurR, BlurG, BlurB, BlurA });
	  int BlurMax = std::max({ BlurR, BlurG, BlurB});

	  cv::Size const ksize(BlurMax * 2 + 1, BlurMax * 2 + 1);
	  retrc.x -= ksize.width / 2;
	  retrc.y -= ksize.height / 2;
	  retrc.width += ksize.width;
	  retrc.height += ksize.height;
	  return 0;
  }

 public:
  int compute(Config const& config, Params const& params, Args const& args,
              cv::Mat& retimg) override try {
    DEBUG_PRINT(__FUNCTION__);

    if (args.invalid(PORT_INPUT)) {
      return 0;
    }

	args.get(PORT_INPUT).copyTo(retimg(args.rect(PORT_INPUT)));

	// Get separated BGR channels
	std::vector<cv::Mat> channels;
	cv::split(retimg, channels);

	const cv::Mat B = channels[0];
	const cv::Mat G = channels[1];
	const cv::Mat R = channels[2];
	const cv::Mat A = channels[3];

    double const BlurR = params.get<double>(PARAM_BLUR_R);
    double const BlurG = params.get<double>(PARAM_BLUR_G);
    double const BlurB = params.get<double>(PARAM_BLUR_B);
    //double const BlurA = params.get<double>(PARAM_BLUR_A);
	    
    if (BlurR > 0) cv::GaussianBlur(R, R, cv::Size(0, 0), BlurR, BlurR);
    if (BlurG > 0) cv::GaussianBlur(G, G, cv::Size(0, 0), BlurG, BlurG);
    if (BlurB > 0) cv::GaussianBlur(B, B, cv::Size(0, 0), BlurB, BlurB);
    //if (BlurA > 0) cv::GaussianBlur(A, A, cv::Size(0, 0), BlurA, BlurA);

	//Merge channels
	cv::merge(channels, retimg);

	cv::cvtColor(retimg, retimg, CV_mRGBA2RGBA);

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
