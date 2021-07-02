#include <iostream>
#include <libstein.h>

int main(int argc, char** argv)
{
    // Check parameters
    libstein::Arguments args(argc, argv);

    libstein::CommandLine cmd;

    auto results = cmd.description("Just an ordinary test.")
        .parameter("a,longa,ENV_LONGA","A value for 'a'", true)
        .parameter("b", "Just another parameter", false)
        .eval(args);

    for (auto& line : results.output)
    {
        std::cout << line << std::endl;
    }

    if (results.valid)
    {
        std::cout << "Success" << std::endl;
        exit(EXIT_SUCCESS);
    }

    std::cout << "Failure" << std::endl;
    exit(EXIT_FAILURE);
}
