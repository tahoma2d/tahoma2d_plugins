#define TNZU_DEFINE_INTERFACE
#include <toonz_utility.hpp>

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
    PARAM_BLUR,
	PARAM_MIX,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Blur_Amount", PARAM_GROUP_DEFAULT, 5, 0, 50},
        ParamPrototype{"Mix", PARAM_GROUP_DEFAULT, 1, 0, 1},
    };
    return &params[i];
  }

  int enlarge(Config const& config, Params const& params,
	  cv::Rect2d& retrc) override {
	  DEBUG_PRINT(__FUNCTION__);

	  retrc = tnzu::make_infinite_rect<double>();
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

    double const BlurAmt = params.get<double>(PARAM_BLUR);
    double const Mix = params.get<double>(PARAM_MIX);

    
    if (BlurAmt > 0 && Mix > 0) {

		//Blur image and copy it
		cv::Mat Blurred;
		cv::GaussianBlur(retimg, Blurred, cv::Size(0, 0), BlurAmt, BlurAmt);

		//Perform Lighten blend
		cv::Mat Blended;
        cv::max(retimg, Blurred, Blended);

		//Mix with original
		double beta = 1 - Mix;
        cv::addWeighted(retimg, beta, Blended, Mix, 1, retimg);
    }
	else 
	{
		retimg = args.get(PORT_INPUT);
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
