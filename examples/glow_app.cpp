#include <cxxopts.hpp>

int main(int argc, char* argv[])
{
    cxxopts::Options options("MyProgram", "One line description of MyProgram");
    options.add_options()("d,debug", "Enable debugging")("f,file", "File name", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);
    return 0;
}
