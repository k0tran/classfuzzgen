#include "render.hpp"

ClassRender::ClassRender(const ClassData &data, const std::string &template_name) : e(), t(e.parse_template(template_name)) {
    d["constructors"] = inja::json::array();
    d["methods"] = inja::json::array();

    for (auto c : data.constr_vec()) {
        inja::json args = inja::json::array();

        for (CXType a : c) {
            CXString s = clang_getTypeSpelling(a);
            dispose_queue.push_back(s);

            args.push_back({
                {"name", clang_getCString(s)},
                {"is_pointer", a.kind == CXType_Pointer},
            });
        }

        d["constructors"].push_back({
            {"args", args},
        });
    }

    for (auto m : data.method_vec()) {
        inja::json args = inja::json::array();

        for (CXType a : m.args) {
            CXString s = clang_getTypeSpelling(a);
            dispose_queue.push_back(s);

            args.push_back({
                {"name", clang_getCString(s)},
                {"is_pointer", a.kind == CXType_Pointer},
            });
        }

        CXString s = clang_getCursorSpelling(m.name);
        d["methods"].push_back({
            {"name", clang_getCString(s)},
            {"args", args}
        });
        dispose_queue.push_back(s);
    }
}

ClassRender &ClassRender::set(const char *key, const char *value) {
    d[key] = value;
    return *this;
}

void ClassRender::render_to(std::ostream &out) {
    e.render_to(out, t, d);
}

ClassRender::~ClassRender() {
    for (auto s : dispose_queue)
        clang_disposeString(s);
}
