#include <iostream>

#include "ast.hpp"
#include "render.hpp"
#include "cli.hpp"

int main(int argc, char **argv) {
    try {
        CLIArgs args(argc, argv);
        if (args.is_err)
            return args.app_exit_code;
        
        ASTData ast(args.header_path, args.compile_args);
        auto class_cursor = ast.find_class(args.class_name.c_str());
        if (!class_cursor.has_value()) {
            std::cout << "Error: class not found" << std::endl;
            return 1;
        }

        ClassData cdata(class_cursor.value());
        // cdata.debug_print(std::cout);

        ClassRender r(cdata, args.template_name);
        r
        .set("class_header", args.header_path.c_str())
        .set("class_name", args.class_name.c_str())
        .render_to(std::cout);

        
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1; 
    }

    return 0;
}