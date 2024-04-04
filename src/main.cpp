#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>

#include "ast.hpp"
#include "class.hpp"

std::string_view file_to_string_view(const char* filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string* str = new std::string(buffer.str());
    return std::string_view(*str);    
}

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
    
        // TODO: get filename from CLI, but for now
        const char *template_name = "template.j2";
        std::string_view contents = file_to_string_view(template_name);
        auto jdata = cdata.render_json(args);
        inja::render_to(std::cout, contents, jdata);

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::logic_error &e) {
        std::cout << "Usage: " << *argv << " <header> <class> ...args_to_compiler..." << std::endl;
        return 1;
    }

    return 0;
}