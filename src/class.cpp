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
    ClassDumpInfo i(constructors, methods);
    clang_visitChildren(class_cursor, class_dump_visitor, (CXClientData)&i);
}

void ClassData::debug_print(std::ostream &out) const {
    out << "Number of constrs: " << constructors.size() << std::endl;
    for (auto &c : constructors) {
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

const std::vector<ConstructorData> &ClassData::constr_vec() const {
    return constructors;
}

const std::vector<MethodData> &ClassData::method_vec() const {
    return methods;
}
