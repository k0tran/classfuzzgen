#pragma once

#include <CLI11.hpp>

class CLIArgs {
public:
    std::string header_path;
    std::string class_name;
    std::string template_name;
    std::string compile_args;
    size_t call_max_length;

    bool is_err;
    int app_exit_code;

    CLIArgs(int argc, char **argv);
};