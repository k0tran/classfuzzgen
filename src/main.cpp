#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>

#include "ast.hpp"
#include "class.hpp"

int main(const int argc, const char *argv[]) {
    try {
        CLIArgs args(argc, argv);

        ASTData ast(args.header_path, args.comp_args);        
        auto class_cursor = ast.find_class(args.class_name);
        if (!class_cursor.has_value()) {
            std::cout << "Error: Class is not found" << std::endl;
            return 1;
        }

        ClassData cdata(class_cursor.value());
        // cdata.debug_print(std::cout);
    
        inja::Environment env;
        inja::Template fuzz_templ = env.parse_template(args.template_name);
        auto jdata = cdata.render_json(args);
        env.render_to(std::cout, fuzz_templ, jdata);

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::logic_error &e) {
        std::cout << "Usage: " << *argv << CLIArgs::ARG_STR << std::endl;
        return 1;
    }

    return 0;
}