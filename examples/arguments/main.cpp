#include <iostream>
#include <libstein/arguments.h>

int main(int argc, char** argv)
{
    // Check parameters
    libstein::Arguments args(argc, argv);

    auto results = args.description("Just an ordinary test.")
        .parameter("a,longa,ENV_LONGA","A value for 'a'", true)
        .parameter("b", "Just another parameter", false)
        .eval();

    while (auto& line : results)
    {
        std::cout << results << std::endl;
    }

    if (results.size() == 0)
    {
        std::cout << "Success" << std::endl;
        exit(EXIT_SUCCESS);
    }

    std::cout << "Failure" << std::endl;
    exit(EXIT_FAILURE);
    // if (    (!args.hasValue("a"))
    //     ||  (!args.hasValue("b")))
    // {
    //     // const char *message =
    //     //     "Usage: test-libstein -a [param1] -b [param2]\n"
    //     //     "\n";

    //     std::cout << message << std::endl;
    //     exit(EXIT_FAILURE);
    // }
}
