OpenToonz Plugin Utility
===

`opentoonz_plugin_utility` is a utility library based on `OpenCV` for `OpenToonz`.
The library is designed for rapidly developing filter and composite effects.

## Directories

* doc
 * documentations
* include
 * C++ header files of the library
* lib
 * output directory of generated library (`.lib` or `.a`)
* samples
 * three simple effect implementations as plugin examples
* src
 * C++ source files of the library
* plugin_sdk
 * low-level interface definitions for developing plugin effects

## How to build the library

`opentoonz_plugin_utility` requires `cmake` as a build system,
and the library uses `OpenCV` for image processing.
Please install them on your platform.

The library build steps:

0. `git clone --recursive` to get source files.
0. `cmake` in that directory.
  - set a directory path contained `OpenCVConfig.cmake` to `OpenCV_DIR`, if needed.
0. `libopentoonz_plugin_utility.{lib,a}` is generated into a `lib` directory.

## Plugin Effect Examples

`amp`, `blur` and `snp` are examples using `opentoonz_plugin_utility`.

### amp

This `amplifier` effect adjusts brightness.
It outputs a result image which multiplies a input image and a `gain` parameter defined in a domain `[0, 1]`.

#### code reading

Let's see `samples/amp/src/main.cpp`.

```
#define TNZU_DEFINE_INTERFACE
#include <toonz_utility.hpp>
```

`toonz_utility.hpp` is a header file of `opentoonz_plugin_utility`.
You have to define `TNZU_DEFINE_INTERFACE` before including `toonz_utility.hpp` at once.
Plugin interfaces, which is refered from a host program, are generated by this definition. 

```
class MyFx : public tnzu::Fx {
```

This is a declaration of the effect class as subclass of `tnzu::Fx`.
The class is a one-to-one correspoindence to a effect on `OpenToonz`.

```cpp
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
```

These are definitions of input ports.
You should define the fixed number of inputs.
`port_name(int i)` should return a name of the `i`-th input.

```cpp
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
```

These are definitions of parameter groups.
Parameter groups are used to manage views for parameters in `OpenToonz`.
`param_group_name(int i)` should return a name of the `i`-th group.

```cpp
enum {
  PARAM_GAIN,
  PARAM_COUNT,
};

int param_count() const override { return PARAM_COUNT; }

ParamPrototype const* param_prototype(int i) const override {
  static std::array<ParamPrototype, PARAM_COUNT> const params = {
      ParamPrototype{"gain", PARAM_GROUP_DEFAULT, 1, 0, 1},
  };
  return &params[i];
}
```

These are definitions of parameters.
You can define the fixed number of parameters.
`param_prototype(int i)` should return a prototype of the `i`-th parameter.
`ParamPrototype` declaration is follows:

```cpp
struct ParamPrototype {
  char const* name;
  int const group;
  double const defvalue;
  double const minvalue;
  double const maxvalue;
};
```

`name`, `group`, `defvalue`, `minvalue` and `maxvalue` are
a parameter name, a group ID, a default value, a minimum value and a maximum value,
respectively.

```cpp
int compute(Config const& config, Params const& params, Args const& args,
            cv::Mat& retimg) override try {
  DEBUG_PRINT(__FUNCTION__);

  if (args.invalid(PORT_INPUT)) {
    return 0;
  }

  double const gain = params.get<double>(PARAM_GAIN);

  retimg = args.get(PORT_INPUT) * gain;

  return 0;
} catch (cv::Exception const& e) {
  DEBUG_PRINT(e.what());
}
```

This is a main part of effect processing.
`config`, `params`, `args` and `retimg` are
environment information, parameter values, input images from ports and a result buffer, respectively.

You can check the `i`-th input port is valid or invalid by `args.valid(int i)` or `args.invalid(int i)`,
and you can get the `i`-th input image as `cv::Mat` by `args.get(int i)`.
The image format which depends on the `OpenToonz` configuration is `CV_8UC4` or `CV_16UC4`.

You can get the `i`-th parameter as type `T` by `params.get<T>(int i)`,
where `T` is `int`, `float`, `double` or `bool`.
Alternatively, you can get the `s` times the parameter value as type `T` by `param.get<T>(int i, double s)`.
Similarly, `params.radian<T>(int i)` returns a `M_PI/180` times value for transforming degree parameter to radian.
Additionally, `params.seed<T>(int i)` returns a random seed for `cv::theRNG().state`,
and `params.rng<T>(int i)` returns a `std::mt19937_64`, when `T` is `std::uint94_t` and a parameter domain is `[0, 1]`.

`retimg` has the same format as input images, and it is a zero cleared image which covers input images.
`args.offset(int)` returns the `i`-th offset position related with `retimg` as `cv::Point2d`,
`args.size(i)` returns a `args.get(i).size()`, and `args.rect(i)` returns a `cv::Rect(args.offset(i), args.size(i))`.
The offset positions should be non-negative and the size of input images should be smaller than a `retimg` size.
This means `args.get(i).copyTo(retimg(args.rect(i)))` is valid in other than fullscreen effects which is described in below.

**Note: `enlarge(...)` and `compute(...)` are called with varying `params` and `args` from multiple threads**

```cpp
namespace tnzu {
PluginInfo const* plugin_info() {
  static PluginInfo const info(TNZU_PP_STR(PLUGIN_NAME),    // name
                               TNZU_PP_STR(PLUGIN_VENDOR),  // vendor
                               "",                          // note
                               "http://dwango.co.jp/");     // helpurl
  return &info;
}

Fx* make_fx() { return new MyFx(); }
}
```

Finally, you have to define functions for telling plugin information to the library.

`plugin_info()` returns a pointer to `PluginInfo`.
`PluginInfo` is defined as follows:

```cpp
struct PluginInfo {
  std::string const name;
  std::string const vendor;
  std::string const identifier;
  std::string const note;
  std::string const helpurl;

  inline PluginInfo(std::string _name, std::string _vendor, std::string _note,
                    std::string _helpurl)
      : name(_name),
        vendor(_vendor),
        identifier(_vendor + "_" + _name),
        note(_note),
        helpurl(_helpurl) {}
};
```

`name`, `vendor`, `note` and `helpurl` are
a effect name, a vendor name, a short description of the effect, and URL for help, respectively.
You can use `[a-zA-Z][a-zA-Z0-9\_]*` for `name` and `vendor`, and you have to choose them for unique `identifier`.
The example defines `PLUGIN_NAME=OpenCV_Blur` and `PLUGIN_VENDOR=DWANGO`.

`make_fx()` returns a new `MyFx` instance.

### blur

This `blur` effect blurs a input image.
It outputs a blurred image by a `cv::GaussianBlur()` function with parameters: `ksize_width`, `ksize_height`, `sigmaX` and `sigmaY`.

#### code reading

Let's see `samples/blur/src/main.cpp`.

The defineition of `blur` is similar to `amp`.

```cpp
#include <opencv2/imgproc/imgproc.hpp>
```

`opencv2/imgproc/imgproc.hpp` is for a `cv::GaussianBlur()` function.

```cpp
enum {
PARAM_KSIZE_WIDTH,
PARAM_KSIZE_HEIGHT,
PARAM_SIGMA_X,
PARAM_SIGMA_Y,
PARAM_COUNT,
};

int param_count() const override { return PARAM_COUNT; }

ParamPrototype const* param_prototype(int i) const override {
static std::array<ParamPrototype, PARAM_COUNT> const params = {
    ParamPrototype{"ksize_width", PARAM_GROUP_DEFAULT, 50, 0, 100},
    ParamPrototype{"ksize_height", PARAM_GROUP_DEFAULT, 50, 0, 100},
    ParamPrototype{"sigmaX", PARAM_GROUP_DEFAULT, 0, 0, 100},
    ParamPrototype{"sigmaY", PARAM_GROUP_DEFAULT, 0, 0, 100},
};
return &params[i];
}
```

These are defintions for parameters.

```cpp
int enlarge(Config const& config, Params const& params,
            cv::Rect2d& retrc) override {
  DEBUG_PRINT(__FUNCTION__);
  cv::Size const ksize(params.get<int>(PARAM_KSIZE_WIDTH) * 2 + 1,
                       params.get<int>(PARAM_KSIZE_HEIGHT) * 2 + 1);
  retrc.x -= ksize.width / 2;
  retrc.y -= ksize.height / 2;
  retrc.width += ksize.width;
  retrc.height += ksize.height;
  return 0;
}
```

`enlarge` function defines a rectangular area to be applied effects to.
`retrc` is initialized by a bounding box which covers bounding boxes of all input images.
The example enlarges `retrc` for margins of blur kernel.

```cpp
int compute(Config const& config, Params const& params, Args const& args,
            cv::Mat& retimg) override try {
  DEBUG_PRINT(__FUNCTION__);

  if (args.invalid(PORT_INPUT)) {
    return 0;
  }

  cv::Size const ksize(params.get<int>(PARAM_KSIZE_WIDTH) * 2 + 1,
                       params.get<int>(PARAM_KSIZE_HEIGHT) * 2 + 1);

  double const sigmaX = params.get<double>(PARAM_SIGMA_X);
  double const sigmaY = params.get<double>(PARAM_SIGMA_Y);

  args.get(PORT_INPUT).copyTo(retimg(args.rect(PORT_INPUT)));
  cv::GaussianBlur(retimg, retimg, ksize, sigmaX, sigmaY);

  return 0;
} catch (cv::Exception const& e) {
  DEBUG_PRINT(e.what());
}
```

This is a definition of filter which applies Gaussan blur.

### snp

This `snp` effect adds SNP (salt-and-pepper) noise to a input image.
It outputs a noised image by random inversion with parameters: a probability `p` and a random `seed` for a random number generator.

#### code reading

Let's see `samples/snp/src/main.cpp`.

The defineition of `snp` is similar to `amp` and `blur`, too.

```cpp
enum {
  PARAM_P,
  PARAM_SEED,
  PARAM_COUNT,
};

int param_count() const override { return PARAM_COUNT; }

ParamPrototype const* param_prototype(int i) const override {
  static std::array<ParamPrototype, PARAM_COUNT> const params = {
      ParamPrototype{"p", PARAM_GROUP_DEFAULT, 0.5, 0, 1},
      ParamPrototype{"seed", PARAM_GROUP_DEFAULT, 0.5, 0, 1},
  };
  return &params[i];
}
```

This is a definition of parameters.

```cpp
int enlarge(Config const& config, Params const& params,
            cv::Rect2d& retrc) override {
  DEBUG_PRINT(__FUNCTION__);
  retrc = tnzu::make_infinite_rect<double>();
  return 0;
}
```

This `enlarge` function defines a fullscreen effect by `tnzu::make_infinite_rect<double>()`. 

```cpp
int compute(Config const& config, Params const& params, Args const& args,
            cv::Mat& retimg) override try {
  DEBUG_PRINT(__FUNCTION__);

  if (args.invalid(PORT_INPUT)) {
    return 0;
  }

  double const p = params.get<double>(PARAM_P);
  std::mt19937_64 rng = params.rng<std::uint64_t>(PARAM_SEED);

  tnzu::draw_image(retimg, args.get(PORT_INPUT), args.offset(PORT_INPUT));

  if (retimg.type() == CV_8UC4) {
    return kernel<cv::Vec4b>(p, retimg, rng);
  }
  else {
    return kernel<cv::Vec4w>(p, retimg, rng);
  }
} catch (cv::Exception const& e) {
  DEBUG_PRINT(e.what());
}
```

This function dispatches a templated function `kernel<Vec4T>(...)`.
Template variable `Vec4T` is used to access pixels values directly, which is determined from the format of `retimg`.

You have to copy input images by `tnzu::draw_image(...)`;
fullscreen effects do not cover all input images, because the size of `retimg` equals to the screen size.

```cpp
template <typename Vec4T, typename RNG>
int MyFx::kernel(double const p, cv::Mat& retimg, RNG& rng) {
  using value_type = typename Vec4T::value_type;

  std::bernoulli_distribution rbern(p);

  for (int y = 0; y < retimg.rows; ++y) {
    Vec4T* scanline = retimg.ptr<Vec4T>(y);
    for (int x = 0; x < retimg.cols; ++x) {
      if (rbern(rng)) {
        int const alpha = scanline[x][3];
        for (int c = 0; c < 3; ++c) {
          // assume premultiplied alpha
          scanline[x][c] =
              cv::saturate_cast<value_type>(alpha - scanline[x][c]);
        }
      }
    }
  }

  return 0;
}
```

This is a main part of effect processing.
This function adds noise sampled from Bernoulli distribution to an image.