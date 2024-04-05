#include "cli.hpp"

const char *CLIArgs::ARG_STR = " <header> <class> <template> ... compiler args ...";

CLIArgs::CLIArgs(const int argc, const char **argv) {
    if (argc < 4)
        throw std::logic_error("");
    
    header_path = argv[1];
    class_name = argv[2];
    template_name = argv[3];
    
    comp_args.n = argc - 4;
    comp_args.args = comp_args.n ? argv + 4 : nullptr;
}