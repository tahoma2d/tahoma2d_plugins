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
    PARAM_CHANNEL,
	PARAM_KEEPALPHA,
    PARAM_COUNT,
  };

  int param_count() const override { return PARAM_COUNT; }

  ParamPrototype const* param_prototype(int i) const override {
    static std::array<ParamPrototype, PARAM_COUNT> const params = {
        ParamPrototype{"Channel", PARAM_GROUP_DEFAULT, 1, 1, 4},
		ParamPrototype{"Keep_Alpha", PARAM_GROUP_DEFAULT, 1, 0, 1},
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

    double const Channel = params.get<double>(PARAM_CHANNEL);
	double const KeepAlpha = params.get<double>(PARAM_KEEPALPHA);

	std::vector<cv::Mat> Channels;
	cv::split(InImg, Channels);

	int CIDX = floor(Channel);
	cv::Mat C = Channels[GetChannelIndex(CIDX)];


	cv::Mat Alpha = KeepAlpha >= 1 ? Channels[3] : cv::Mat(InImg.size(), CV_8UC1, cv::Scalar(255, 255, 255));

	std::vector<cv::Mat> Combined;
	Combined.push_back(C);
	Combined.push_back(C);
	Combined.push_back(C);
	Combined.push_back(Alpha);

	cv::merge(Combined, retimg);

    return 0;
  } catch (cv::Exception const& e) {
    DEBUG_PRINT(e.what());
  }



  int GetChannelIndex(int InChannel);
};

int MyFx::GetChannelIndex(int InChannel)
{
	int outval;

	switch (InChannel)
	{
	case 1: outval = 2;
		break;
	case 2: outval = 1;
		break;
	case 3: outval = 0;
		break;
	case 4: outval = 3;
	}

	return outval;
}

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
