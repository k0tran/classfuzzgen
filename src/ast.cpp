#include <cstring>

#include "ast.hpp"

std::vector<std::string> split(std::string &data) {
    std::istringstream iss(data);
    std::string tmp;
    std::vector<std::string> res;

    while (iss >> tmp)
        res.push_back(tmp);
    
    return res;
}


ASTData::ASTData(std::string &header_path, std::string &comp_args) {
    auto splited = split(comp_args);
    std::vector<const char *> args;
    for (auto &s : splited)
        args.push_back(s.c_str());

    index = clang_createIndex(0, 0);
    translation_unit = clang_parseTranslationUnit(
        index,
        header_path.c_str(),
        args.data(),
        args.size(),
        0,
        0,
        CXTranslationUnit_None
    );

    if (!translation_unit)
        throw std::runtime_error("Cannot create translation unit");
    
    root_cursor = clang_getTranslationUnitCursor(translation_unit);
}


class ClassSearchInfo {
public:
    const char *name;
    std::optional<CXCursor> cursor;
    
    ClassSearchInfo(const char *class_name) : name(class_name), cursor({}) {}
};


CXChildVisitResult class_search_visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(cursor) == CXCursor_ClassDecl) {
        CXString current_class = clang_getCursorSpelling(cursor);
        ClassSearchInfo *i = (ClassSearchInfo * )client_data;

        if (strcmp(clang_getCString(current_class), i->name) == 0) {
            i->cursor.emplace(cursor);
            clang_disposeString(current_class);
            return CXChildVisit_Break;
        }

        clang_disposeString(current_class);
    }
    return CXChildVisit_Recurse;
}

std::optional<CXCursor> ASTData::find_class(const char *class_name) const {
    ClassSearchInfo i(class_name);
    clang_visitChildren(root_cursor, class_search_visitor, (CXClientData)&i);
    return i.cursor;
}

ASTData::~ASTData() {
    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(index);
}

