#define TNZU_DEFINE_INTERFACE
#include <toonz_utility.hpp>

class MyFx : public tnzu::Fx {
 public:
  //
  // PORT
  //
  enum {
	PORT_INPUT_0,
    PORT_INPUT_1,
	PORT_INPUT_2,
	PORT_INPUT_3,
	PORT_INPUT_4,
	PORT_INPUT_5,
	PORT_INPUT_6,
	PORT_INPUT_7,
	PORT_INPUT_8,
	PORT_INPUT_9,
    PORT_COUNT,
  };

  int port_count() const override { return PORT_COUNT; }

  char const* port_name(int i) const override {
    static std::array<char const*, PORT_COUNT> names = {
		"Input_0","Input_1","Input_2","Input_3","Input_4",
		"Input_5","Input_6","Input_7","Input_8","Input_9",
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
    PARAM_INDEX,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Index", PARAM_GROUP_DEFAULT, 0, 0, 9},
    };
    return &params[i];
  }

 public:
  int compute(Config const& config, Params const& params, Args const& args,
              cv::Mat& retimg) override try {
    DEBUG_PRINT(__FUNCTION__);


	int idx = std::floor(params.get<float>(PARAM_INDEX));
	retimg = args.get(idx);

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
