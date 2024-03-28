#pragma once

#include <stdexcept>

class CLIArgs {
public:
    struct CompArgs {
        const char **args;
        size_t n;
    };

    const char *header_path;
    const char *class_name;
    CompArgs comp_args;

    CLIArgs(const int argc, const char **argv) {
        if (argc < 3)
            throw std::logic_error("");
        
        header_path = argv[1];
        class_name = argv[2];

        comp_args.n = argc - 3;
        comp_args.args = comp_args.n ? argv + 3 : nullptr;
    }
};
