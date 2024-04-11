#pragma once

#include <optional>
#include <clang-c/Index.h>

#include "cli.hpp"


class ASTData {
public:
    ASTData(std::string &header_path, std::string &comp_args);
    
    std::optional<CXCursor> find_class(const char *class_name) const;

    ~ASTData();

private:
    CXIndex index;
    CXTranslationUnit translation_unit;
    CXCursor root_cursor;
};