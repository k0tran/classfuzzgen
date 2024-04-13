#pragma once

#include <inja.hpp>

#include "class.hpp"

class ClassRender {
public:
    ClassRender(const ClassData &data, const std::string &template_name);

    ClassRender &set(const char *key, const char *value);
    void render_to(std::ostream &out);

    ~ClassRender();

private:
    inja::json d;
    std::vector<CXString> dispose_queue;
    inja::Environment e;
    inja::Template t;
};
