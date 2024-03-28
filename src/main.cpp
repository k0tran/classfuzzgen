#include <iostream>

#include "ast.hpp"

int main(const int argc, const char *argv[]) {
    try {
        CLIArgs args(argc, argv);

        ASTData ast(args.header_path, args.comp_args);        
        auto actual_class_holy_cow = ast.find_class(args.class_name);
        if (!actual_class_holy_cow.has_value()) {
            std::cout << "Error: Class is not found" << std::endl;
            return 1;
        }

        // 3. Extract valuable info (class data and connected classes maybe)
        // 4. Dump everything into stdout?
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::logic_error &e) {
        std::cout << "Usage: " << *argv << " <header> <class> ...args_to_compiler..." << std::endl;
        return 1;
    }

    return 0;
}