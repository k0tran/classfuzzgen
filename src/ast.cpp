#include <cstring>

#include "ast.hpp"


ASTData::ASTData(const char *header_path, CLIArgs::CompArgs args) {
    index = clang_createIndex(0, 0);
    translation_unit = clang_parseTranslationUnit(
        index,
        header_path,
        args.args,
        args.n,
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

