#include "makemeglow/glow.h"
#include "makemeglow/png.h"

#include <cxxopts.hpp>

#include <exception>
#include <numeric>

namespace {

std::string joinText(const std::vector<std::string>& strings)
{
    return std::accumulate(strings.cbegin(), strings.cend(), std::string{""},
        [](const std::string& l, const std::string& r) {
            return l + (l.empty() ? "" : " ") + r;
        });
}

glow::Color parseColor(const std::string& s)
{
    if (s.size() < 7 || s[0] != '#') {
        throw std::runtime_error{"Color format is #RRGGBB{AA}"};
    }

    uint32_t value = 0;
    const auto substr = s.substr(1);
    if (substr.size() == 6) {
        value = strtoul(substr.c_str(), nullptr, /* basse*/ 16);
        value = (value << 8) | 0xff;
    } else if (substr.size() == 8) {
        value = strtoul(substr.c_str(), nullptr, /* basse*/ 16);
    } else {
        throw std::runtime_error{"Color format is #RRGGBB{AA}"};
    }

    return glow::Color{value};
}

void validateArgs(const cxxopts::ParseResult& args)
{
    if (args.count("input") == 0) {
        throw std::runtime_error{"Input is required param"};
    }
    if (args.count("font") == 0) {
        throw std::runtime_error{"Font is required param"};
    }
}

cxxopts::Options createProgramOptions()
{
    cxxopts::Options options("glow_app", "./glow_app");

    // turn off clang format for options list
    // clang-format off
    options
        .allow_unrecognised_options()
        .add_options()
        ("help", "Print help")
        ("input", "Input", cxxopts::value<std::vector<std::string>>())
        ("o,output", "Outout file",
            cxxopts::value<std::string>()->default_value("./text.png"))
        ("f,font", "Font file", cxxopts::value<std::string>())
        ("c,color", "Text color color in format #RRGGBB{AA}",
            cxxopts::value<std::string>()->default_value("#faafff"))
        ("b,background", "Background color in format #RRGGBB{AA}",
            cxxopts::value<std::string>()->default_value("#100f00"))
        ("s,size", "Text size in pixels",
            cxxopts::value<size_t>()->default_value("20"))
        ("radius", "Raduis of Gaussian blur in pixels",
            cxxopts::value<size_t>()->default_value("5"))
        ("sigma", "Sigma parameter of Gaussian blur",
            cxxopts::value<float>()->default_value("1.f"));
    // clang-format on

    options.parse_positional({"input"});

    return options;
}

struct RasterParams {
    std::string text;
    std::string font;
    size_t pixelSize;
    glow::Color textColor;
    glow::Color backgroundColor;
    size_t radius;
    float sigma;
};

RasterParams getRasterParams(const cxxopts::ParseResult& args)
{
    RasterParams params;

    params.text = joinText(args["input"].as<std::vector<std::string>>());
    params.font = args["font"].as<std::string>();
    params.pixelSize = args["size"].as<size_t>();
    params.textColor = parseColor(args["color"].as<std::string>());
    params.backgroundColor = parseColor(args["background"].as<std::string>());
    params.radius = args["radius"].as<size_t>();
    params.sigma = args["sigma"].as<float>();

    return params;
}

glow::ColorBuffer rasterize(const RasterParams& params)
{
    return glow::rasterize(params.text, params.font,
        params.pixelSize,
        params.textColor, params.backgroundColor);
}

} // namespace

int main(int argc, char* argv[])
{
    try {
        auto options = createProgramOptions();
        auto args = options.parse(argc, argv);
        if (args.count("help")) {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(0);
        }

        validateArgs(args);
        auto params = getRasterParams(args);

        glow::writePng(args["output"].as<std::string>(), rasterize(params));

        return 0;
    } catch (const cxxopts::OptionException& e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}
