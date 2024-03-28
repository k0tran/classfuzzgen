#pragma once

#include <optional>
#include <clang-c/Index.h>

#include "cli.hpp"


class ASTData {
public:
    ASTData(const char *header_path, CLIArgs::CompArgs args);
    
    std::optional<CXCursor> find_class(const char *class_name) const;

    ~ASTData();

private:
    CXIndex index;
    CXTranslationUnit translation_unit;
    CXCursor root_cursor;
};