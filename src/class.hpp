#pragma once

#include <vector>
#include <iostream>
#include <clang-c/Index.h>
#include "../include/inja.hpp"

#include "cli.hpp"

// ConstructorData contains only argument names
typedef std::vector<CXType> ConstructorData;

// Methods contains their names and argument names
class MethodData {
public:
    CXCursor name;
    std::vector<CXType> args;

    MethodData(CXCursor method_name) : name(method_name), args({}) {}
};

class ClassData {
public:
    ClassData(CXCursor class_cursor);

    void debug_print(std::ostream &out) const;

    inja::json render_json(CLIArgs &args) const;
private:
    std::vector<ConstructorData> contructors;
    std::vector<MethodData> methods;
};