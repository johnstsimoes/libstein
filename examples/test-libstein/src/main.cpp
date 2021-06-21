#include <iostream>
#include <libstein/arguments.h>

int main(int argc, char** argv)
{
    // Check parameters
    libstein::Arguments args(argc, argv);

    if (    (!args.hasValue("a"))
        ||  (!args.hasValue("b")))
    {
        const char *message =
            "Usage: test-libstein -a [param1] -b [param2]\n"
            "\n";

        std::cout << message << std::endl;
        exit(EXIT_FAILURE);
    }
}
