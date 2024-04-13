#pragma once

#include <vector>
#include <iostream>
#include <clang-c/Index.h>

typedef std::vector<CXType> ConstructorData;

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

    const std::vector<ConstructorData> &constr_vec() const;
    const std::vector<MethodData> &method_vec() const;

private:
    std::vector<ConstructorData> constructors;
    std::vector<MethodData> methods;
};