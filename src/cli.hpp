#pragma once

#include <stdexcept>

class CLIArgs {
public:
    struct CompArgs {
        const char **args;
        size_t n;
    };

    static const char *ARG_STR;

    const char *header_path;
    const char *class_name;
    const char *template_name;
    CompArgs comp_args;

    CLIArgs(const int argc, const char **argv);
};
