#include "cli.hpp"

CLIArgs::CLIArgs(int argc, char **argv) {
    CLI::App app("Harness function generator for classes");
    argv = app.ensure_utf8(argv);

    app.add_option("-H,--header", header_path, "Header path to the class")->required()->check(CLI::ExistingFile);
    app.add_option("-c,--class", class_name, "Name of the target class")->required();
    app.add_option("-t,--template", template_name, "Path to the jinja2 template")->check(CLI::ExistingFile)->default_str("template.j2");
    app.add_option("-a,--compile-args", compile_args, "Compile args to clang")->default_str("");
    app.add_option("-l,--call-max-length", call_max_length, "Maximum call chain length")->default_val(10);

    app_exit_code = 0;
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        app_exit_code = app.exit(e);
    }
}
