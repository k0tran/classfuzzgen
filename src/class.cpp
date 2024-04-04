#include "class.hpp"


class ClassDumpInfo {
public:
    std::vector<ConstructorData> &c;
    std::vector<MethodData> &m;
    
    ClassDumpInfo(
        std::vector<ConstructorData> &constructors,
        std::vector<MethodData> &methods
    ) : c(constructors), m(methods) {}
};


CXChildVisitResult class_dump_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    ClassDumpInfo *i = (ClassDumpInfo *)client_data;
    
    if (clang_getCursorKind(cursor) == CXCursor_Constructor) {
        CXType c = clang_getCursorType(cursor);
        ConstructorData cdata;

        for (size_t i = 0; i < clang_getNumArgTypes(c); ++i)
            cdata.push_back(clang_getArgType(c, i));
        
        i->c.push_back(cdata);
    } else if (clang_getCursorKind(cursor) == CXCursor_CXXMethod) {
        CXType m = clang_getCursorType(cursor);
        MethodData mdata(cursor);

        for (size_t i = 0; i < clang_getNumArgTypes(m); ++i)
            mdata.args.push_back(clang_getArgType(m, i));
        
        i->m.push_back(mdata);
    }

    return CXChildVisit_Continue;
}


ClassData::ClassData(CXCursor class_cursor) {
    ClassDumpInfo i(contructors, methods);
    clang_visitChildren(class_cursor, class_dump_visitor, (CXClientData)&i);
}

void ClassData::debug_print(std::ostream &out) const {
    out << "Number of constrs: " << contructors.size() << std::endl;
    for (auto &c : contructors) {
        for (auto &a : c){
            CXString tmp = clang_getTypeSpelling(a);
            out << clang_getCString(tmp) << " ";
            clang_disposeString(tmp);
        }
        out << std::endl;
    }

    out << "Number of methods: " << methods.size() << std::endl;
    for (auto &m : methods) {
        CXString tmp = clang_getCursorSpelling(m.name);
        out << clang_getCString(tmp) << ": ";
        clang_disposeString(tmp);

        for (auto &a : m.args){
            tmp = clang_getTypeSpelling(a);
            out << clang_getCString(tmp) << " ";
            clang_disposeString(tmp);
        }

        out << std::endl;
    }
}

inja::json ClassData::render_json(CLIArgs &args) const {
    inja::json d;
    d["class_header"] = args.header_path;
    d["class_name"] = args.class_name;
    d["constructors"] = inja::json::array();
    d["methods"] = inja::json::array();

    for (size_t i = 0; i < contructors.size(); ++i) {
        d["constructors"][i] = inja::json::array();

        for (size_t j = 0; j < contructors[i].size(); ++j) {
            auto s = clang_getTypeSpelling(contructors[i][j]);
            d["constructors"][i].push_back(clang_getCString(s));
            clang_disposeString(s);
        }
    }

    for (size_t i = 0; i < methods.size(); ++i) {
        auto s = clang_getCursorSpelling(methods[i].name);
        d["methods"][i]["name"] = clang_getCString(s);
        d["methods"][i]["args"] = inja::json::array();
        clang_disposeString(s);
        
        for (size_t j = 0; j < methods[i].args.size(); ++j) {
            s = clang_getTypeSpelling(methods[i].args[j]);
            d["methods"][i]["args"].push_back(clang_getCString(s));
            clang_disposeString(s);
        }
    }


    return d;
}
