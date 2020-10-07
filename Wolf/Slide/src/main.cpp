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
    PARAM_X,
	PARAM_Y,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"X", PARAM_GROUP_DEFAULT, 0, 0, 100},
		ParamPrototype{"Y", PARAM_GROUP_DEFAULT, 0, 0, 100},
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

	const double TX = (args.size(0).width / 100) * params.get<double>(PARAM_X);
	const double TY = (args.size(0).height / 100) * params.get<double>(PARAM_Y);
	double Ar[6] = { 1, 0, TX, 0, 1, TY };
	cv::Mat TMatrix = cv::Mat(2, 3, CV_64FC1, Ar);
	cv::warpAffine(InImg, retimg, TMatrix, InImg.size(), cv::INTER_LINEAR, cv::BORDER_WRAP);

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
