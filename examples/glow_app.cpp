#include "makemeglow/color.h"
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

void validateArgs(const cxxopts::ParseResult& args)
{
    if (args.count("input") == 0) {
        throw cxxopts::OptionException{"Input is required param"};
    }
    if (args.count("font") == 0) {
        throw cxxopts::OptionException{"Font is required param"};
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
        ("h,help", "Print help")
        ("input", "Input", cxxopts::value<std::vector<std::string>>())
        ("o,output", "Outout file",
            cxxopts::value<std::string>()->default_value("./text.png"))
        ("f,font", "Font file", cxxopts::value<std::string>())
        ("c,color", "Text color color in format #RRGGBB",
            cxxopts::value<std::string>()->default_value("faafff"))
        ("b,background", "Background color in format #RRGGBB",
            cxxopts::value<std::string>()->default_value("100f00"))
        ("s,size", "Text size in pixels",
            cxxopts::value<size_t>()->default_value("20"))
        ("kernel", "Kernel size of Gaussian blur",
            cxxopts::value<size_t>()->default_value("9"))
        ("sigma", "Sigma parameter of Gaussian blur",
            cxxopts::value<float>()->default_value("3.0"));
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
    size_t kernelSize;
    float sigma;
};

RasterParams getRasterParams(const cxxopts::ParseResult& args)
{
    RasterParams params;

    params.text = joinText(args["input"].as<std::vector<std::string>>());
    params.font = args["font"].as<std::string>();
    params.pixelSize = args["size"].as<size_t>();
    params.textColor = glow::parseColor(args["color"].as<std::string>());
    params.backgroundColor = glow::parseColor(args["background"].as<std::string>());
    params.kernelSize = args["kernel"].as<size_t>();
    params.sigma = args["sigma"].as<float>();

    return params;
}

glow::ColorBuffer rasterize(const RasterParams& params)
{
    return glow::makeRasterizer(params.font)
        ->rasterize(params.text, params.pixelSize,
            params.textColor, params.backgroundColor,
            glow::GlowParams{params.kernelSize, params.sigma});
}

} // namespace

int main(int argc, char* argv[])
{
    try {
        auto options = createProgramOptions();
        const auto args = options.parse(argc, argv);

        if (args.count("help")) {
            std::cout << options.help({"", "Group"}) << std::endl;
            exit(0);
        }

        validateArgs(args);
        const auto params = getRasterParams(args);

        glow::writePng(args["output"].as<std::string>(), rasterize(params));

        return 0;
    } catch (const cxxopts::OptionException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "oops...\n";
    }
    exit(1);
}
