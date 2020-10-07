#define TNZU_DEFINE_INTERFACE
#include <stdio.h>
#include <toonz_utility.hpp>
#include <toonz_params.h>

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
    PARAM_SEPARATION_H,
    PARAM_SEPARATION_V,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Separation_H", PARAM_GROUP_DEFAULT, 5, 0, 200},
        ParamPrototype{"Separation_V", PARAM_GROUP_DEFAULT, 0, 0, 200},
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
   
    double const SeparationH = params.get<double>(PARAM_SEPARATION_H);
    double const SeparationV = params.get<double>(PARAM_SEPARATION_V);

	// Get separated BGR channels
    std::vector<cv::Mat> channels;
    cv::split(InImg, channels);

    cv::Mat B = channels[0];
    cv::Mat G = channels[1];
    cv::Mat R = channels[2];

	//Move R channel
    translateImg(R, -SeparationH, -SeparationV);
	//Move B channel
    translateImg(B, SeparationH, SeparationV);

	//Merge channels
    cv::merge(channels, retimg);

    return 0;
  } catch (cv::Exception const& e) {
    DEBUG_PRINT(e.what());
  }
  
  //https://stackoverflow.com/a/26766505
  cv::Mat translateImg(cv::Mat& img, int offsetx, int offsety) 
  {
    cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
    cv::warpAffine(img, img, trans_mat, img.size());
    return img;
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
}  // namespace tnzu
