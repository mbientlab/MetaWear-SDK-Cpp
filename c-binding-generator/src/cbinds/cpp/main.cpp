#include <cstring>
#include <clang-c/Index.h>  // This is libclang.
#include <fstream>
#include <iostream>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Builder.h"
#include "cli.h"
#include "DummyGenerator.h"

#ifdef _WIN32
#include <windows.h> 
#else
#include <dlfcn.h>
#endif

using namespace std;
using namespace mbientlab::cbinds;

enum CursorState {
    NONE,
    ENUM,
    STRUCT,
    FUNCTION,
    FUNCTION_POINTER,
    STRUCT_FIELD
};

static stack<CursorState> state;

static builder::CHeaderFileBuilder cHeaderBuilder;
static builder::EnumBuilder* enumBuilder = nullptr;
static stack<builder::TypeBuilder> typeBuilder;
static stack<builder::VariableBuilder> varBuilder;
static builder::FunctionBuilder* fnBuilder = nullptr;
static builder::StructBuilder* structBuilder = nullptr;

string toString(const CXString& str) {
    string copy(clang_getCString(str));
    clang_disposeString(str);
    return copy;
}

int main(int argc, char** argv) {
    int nArgs;
    char** clangArgs;
    string libpath, outpath, creator, input;

    CliBuilder *cli= CliBuilder::getBuilder();
    cli->addOption(Option("--generator-lib", 0, 1, [&libpath](const Arguments &args) { libpath= args.asString(0); })
            .withArgName("libname").withDescription("Custom dll/so defining a bindings generator"))
        .addOption(Option("--generator-creator", 0, 1, [&creator](const Arguments &args) { creator= args.asString(0); })
            .withArgName("name").withDescription("Function to call into the dll/so"))
        .addOption(Option("-o", 0, 1, [&outpath](const Arguments &args) { outpath= args.asString(0); })
            .withLongOpt("--output").withArgName("path").withDescription("Path to write the output to"))
        .addOption(Option("-h", [&cli](const Arguments &args) { cli->displayUsage(); })
            .withLongOpt("--help").withDescription("Displays this help message and exits"))
        .addOption(Option("-f", 0, 1, [&input](const Arguments &args) { input = args.asString(0); })
            .withLongOpt("--file").withArgName("path").withDescription("Header file to parse").withRequired(true))
        .addOption(Option("--cxx-flags", ' ', Option::NARGS_UNLIMITED, [&clangArgs,&nArgs](const Arguments &args) -> void {
            nArgs = static_cast<int>(args.size());
            clangArgs = new char*[nArgs];
            for (size_t i = 0; i < args.size(); i++) {
                size_t len = args.asString(i).size() + 1;
                clangArgs[i] = new char[len];
                memcpy(clangArgs[i], args.asString(i).c_str(), len);
            }
        }).withArgName("flags").withDescription("Clang compilation flags to use").withRequired(true))
        .setUsage("cbinds --cxx-flags <flags> -f <file> [options]");
    cli->parse(argc, argv);

    CXIndex index = clang_createIndex(0, 0);
    static CXTranslationUnit unit;
    auto result = clang_parseTranslationUnit2(index, input.c_str(), clangArgs, nArgs, nullptr, 0, CXTranslationUnit_None, &unit);
    if (result) {
        cerr << "Unable to parse translation unit (status = " << result << ")" << endl;
        exit(-1);
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
        auto location = clang_getCursorLocation(c);
        if (clang_Location_isInSystemHeader(location)) {
            return CXChildVisit_Continue;
        }

        CXChildVisitResult result = CXChildVisit_Continue;
        auto kind = clang_getCursorKind(c);
//        cout << toString(clang_getCursorKindSpelling(kind)) << ", " << toString(clang_getCursorSpelling(c)) << endl;
        switch(kind) {
            case CXCursor_EnumConstantDecl: {
                enumBuilder->addEntry(clang_Cursor_getRawCommentText(c), clang_getCursorSpelling(c), clang_getEnumConstantDeclValue(c));
                result = CXChildVisit_Recurse;
                break;
            }
            case CXCursor_TypedefDecl: {
                auto type = clang_getTypedefDeclUnderlyingType(c);
                if (type.kind == CXType_Pointer) {
                    string typeString = toString(clang_getTypeSpelling(type));
                    if (typeString.find("(") != string::npos) {
                        state.push(FUNCTION_POINTER);
                        typeBuilder.emplace(clang_getCursorSpelling(c));
                        typeBuilder.top().setFnPointer();
                        typeBuilder.top().setComment(clang_Cursor_getRawCommentText(c));
                    }
                    result = CXChildVisit_Recurse;
                } else if (type.kind == CXType_Elaborated) {
                    if (state.top() == ENUM) {
                        enumBuilder->setName(clang_getCursorSpelling(c));
                        cHeaderBuilder.addEnum(enumBuilder->build());
                        delete enumBuilder;
                        state.pop();
                    } else {
                        Variable* var = nullptr;
                        if (state.top() == FUNCTION_POINTER) {
                            varBuilder.top().setType(*(typeBuilder.top().build()));
                            var = varBuilder.top().build();
                            typeBuilder.pop();
                            varBuilder.pop();
                            state.pop();
                        }
                        if (state.top() == STRUCT) {
                            if (var != nullptr) {
                                structBuilder->addField(var);
                            }
                            structBuilder->setName(clang_getCursorSpelling(c));
                            cHeaderBuilder.addStruct(structBuilder->build());
                            delete structBuilder;
                            state.pop();
                        }
                    }
                }
                break;
            }
            case CXCursor_EnumDecl: {
                state.push(ENUM);
                enumBuilder = new builder::EnumBuilder();
                enumBuilder->setComment(clang_Cursor_getRawCommentText(c));
                result = CXChildVisit_Recurse;
                break;
            }
            case CXCursor_ParmDecl: {
                auto type = clang_getCursorType(c);
                string typeString(toString(clang_getTypeSpelling(type)));

                builder::TypeBuilder builder(clang_getTypeSpelling(type));
                switch(type.kind) {
                    case CXType_Pointer:
                        builder.setPointer();
                        break;
                    case CXType_ConstantArray:
                    case CXType_IncompleteArray:
                        builder.setArray(clang_getArraySize(type));
                        break;
                    default:
                        break;
                }

                varBuilder.emplace(clang_getCursorSpelling(c));
                varBuilder.top().setType(*builder.build());

                if (state.top() == FUNCTION_POINTER) {
                    typeBuilder.top().addParameter(varBuilder.top().build());
                    varBuilder.pop();
                } else if (state.top() == STRUCT) {
                    varBuilder.top().setComment(clang_Cursor_getRawCommentText(c));
                    structBuilder->addField(varBuilder.top().build());                        
                    varBuilder.pop();
                }
                break;
            }
            case CXCursor_StructDecl: {
                auto cDef = clang_getCursorDefinition(c);
                structBuilder = new builder::StructBuilder(clang_getCursorSpelling(c));
                structBuilder->setComment(clang_Cursor_getRawCommentText(c));
                if (clang_Cursor_isNull(cDef)) {
                    structBuilder->setForwardDecl();
                    cHeaderBuilder.addStruct(structBuilder->build());
                    delete structBuilder;
                } else {
                    result = CXChildVisit_Recurse;
                    state.push(STRUCT);
                }
                break;
            }
            case CXCursor_FieldDecl: {
                if (state.top() == FUNCTION_POINTER) {
                    auto type = dynamic_cast<FunctionPointer*>(typeBuilder.top().build());
                    typeBuilder.pop();
                    state.pop();

                    if (!state.empty() && state.top() == STRUCT) {
                        varBuilder.top().setComment(clang_Cursor_getRawCommentText(c));
                        varBuilder.top().setType(*type);
                        auto ptr = varBuilder.top().build();
                        varBuilder.pop();

                        structBuilder->addField(ptr);
                    } else {
                        cHeaderBuilder.addFunctionPtr(type);
                    }
                }
                auto type = clang_getCursorType(c);
                string typeString = toString(clang_getTypeSpelling(type));
                result = CXChildVisit_Recurse;
                if (typeString.find("(") != string::npos) {
                    state.push(FUNCTION_POINTER);
                    
                    varBuilder.emplace(clang_getCursorSpelling(c));
                    varBuilder.top().setComment(clang_Cursor_getRawCommentText(c));
                    typeBuilder.emplace(clang_getTypeSpelling(type));
                    typeBuilder.top().setFnPointer();
                } else {
                    if (state.top() == STRUCT) {
                        builder::VariableBuilder vbuilder(clang_getCursorSpelling(c));
                        builder::TypeBuilder tbuilder(clang_getTypeSpelling(type));
                        switch(type.kind) {
                            case CXType_Pointer:
                                tbuilder.setPointer();
                                break;
                            case CXType_ConstantArray:
                            case CXType_IncompleteArray:
                                tbuilder.setArray(clang_getArraySize(type));
                                break;
                            default:
                                break;
                        }
                        vbuilder.setType(*(tbuilder.build()));
                        vbuilder.setComment(clang_Cursor_getRawCommentText(c));
                        structBuilder->addField(vbuilder.build());
                        result = CXChildVisit_Continue;
                    }
                }
                break;
            }
            case CXCursor_FunctionDecl: {
                if (state.size() > 0 && state.top() != FUNCTION) {
                    if (state.top() == FUNCTION_POINTER) {
                        auto type = dynamic_cast<FunctionPointer*>(typeBuilder.top().build());

                        state.pop();

                        if (!state.empty() && state.top() == STRUCT) {
                            varBuilder.top().setType(*type);
                            structBuilder->addField(varBuilder.top().build());
                            varBuilder.pop();
                        } else {
                            cHeaderBuilder.addFunctionPtr(type);
                        }
                        typeBuilder.pop();
                    } else {
                        state.pop();
                    }
                }

                if (clang_getCursorVisibility(c) == CXVisibility_Default) {
                    state.push(FUNCTION);
                    fnBuilder = new builder::FunctionBuilder(clang_getCursorSpelling(c));
                    fnBuilder->setComment(clang_Cursor_getRawCommentText(c));

                    for(int i = 0; i < clang_Cursor_getNumArguments(c); i++) {
                        auto arg = clang_Cursor_getArgument(c, i);
                        auto type = clang_getCursorType(arg);

                        bool isArray = type.kind == CXType_ConstantArray || type.kind == CXType_IncompleteArray;
                        fnBuilder->addParameter(clang_getCursorSpelling(arg), clang_getTypeSpelling(type), type.kind == CXType_Pointer, 
                                isArray, isArray ? clang_getArraySize(type) : -1);
                    }

                    auto resType = clang_getResultType(clang_getCursorType(c));
                    fnBuilder->setReturnType(clang_getTypeSpelling(resType), resType.kind == CXType_Pointer);
                    cHeaderBuilder.addFunction(fnBuilder->build());

                    delete fnBuilder;

                    state.pop();
                }
                break;
            }
            case CXCursor_TypeRef: {
                if (!state.empty()) {
                    auto type = clang_getCursorType(c);
                    builder::TypeBuilder builder(clang_getTypeSpelling(type));
                    if (type.kind == CXType_Pointer) {
                        builder.setPointer();
                    }
                    auto typeObj = builder.build();
                    if (varBuilder.empty()) {
                        if (state.top() == FUNCTION_POINTER) {
                            typeBuilder.top().setReturnType(builder.build());
                        } else {
                            throw runtime_error("varBuilder is empty while parser is in function pointer state");
                        }
                    } else {
                        varBuilder.top().setType(*typeObj);
                        if (state.top() == FUNCTION_POINTER) {
                            typeBuilder.top().addParameter(varBuilder.top().build());
                            varBuilder.pop();
                        }
                    }
                }
                break;
            }
            case CXCursor_VarDecl: {
                auto type = clang_getCursorType(c);
                auto decl = clang_getTypeDeclaration(type);
                auto eval = clang_Cursor_Evaluate(c);

                builder::TypeBuilder builder(clang_getCursorSpelling(decl));
                builder::ConstBuilder constBuilder(clang_getCursorSpelling(c), builder.build());
                constBuilder.setComment(clang_Cursor_getRawCommentText(c));
                
                switch(clang_EvalResult_getKind(eval)) {
                    case CXEval_Int: {
                        stringstream sstream;
                        sstream << clang_EvalResult_getAsLongLong(eval);
                        constBuilder.setValue(sstream.str());
                        break;
                    }
                    case CXEval_UnExposed: {
                        CXSourceRange range = clang_getCursorExtent(c);
                        CXToken *clangTokens = 0;
                        unsigned int nTokens = 0;                
                        clang_tokenize(unit, range, &clangTokens, &nTokens);

                        for (unsigned int i = 0; i < nTokens; i++) {
                            if(clang_getTokenKind(clangTokens[i]) == CXToken_Literal) {
                                CXString spelling = clang_getTokenSpelling(unit, clangTokens[i]);

                                constBuilder.addValueComponent(clang_getCString(spelling));
                            }
                        }
                        clang_disposeTokens(unit, clangTokens, nTokens);
                        break;
                    }
                    default:
                        break;
                }

                cHeaderBuilder.addConstant(constBuilder.build());
            }
            case CXCursor_UnexposedDecl:
                if (!state.empty() && state.top() == FUNCTION_POINTER) {
                    auto type = dynamic_cast<FunctionPointer*>(typeBuilder.top().build());
                    typeBuilder.pop();
                    cHeaderBuilder.addFunctionPtr(type);

                    state.pop();
                }
                result = CXChildVisit_Recurse;
                break;
            default:
                break;
        }
        return result;
    }, nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    
    while(!typeBuilder.empty()) {
        cHeaderBuilder.addFunctionPtr(dynamic_cast<FunctionPointer*>(typeBuilder.top().build()));
        typeBuilder.pop();
    }

    ofstream fileout;
    ostream* os;
    if (outpath.empty()) {
        os = &cout;
    } else {
        fileout.open(outpath.c_str());
        if (!fileout.is_open()) {
            string error("Could not open file for writing: " + outpath);
            throw runtime_error(error.c_str());
        }
        os = &fileout;
    }

    Generator* generator;
#ifdef _WIN32
    HMODULE dllHandle = nullptr;
#else
    void* dllHandle = nullptr;
#endif

    if (libpath.empty()) {
        generator = new DummyGenerator();
    } else {
#ifdef _WIN32
        wstring temp = wstring(libpath.begin(), libpath.end());
        LPCWSTR dataLibWStr = temp.c_str();
        dllHandle = LoadLibrary(dataLibWStr);

        if (!dllHandle) {
            throw runtime_error("Error loading library: " + libpath);
        }
        generator = reinterpret_cast<Generator*(*)()>(GetProcAddress(dllHandle, creator.c_str()))();
        if (!generator) {
            FreeLibrary(dllHandle);
            throw runtime_error("Error loading function: " + creator);
        }
#else
        dllHandle= dlopen(libpath.c_str(), RTLD_LAZY);
        if (!dllHandle) {
            throw runtime_error(dlerror());
        }
        generator = reinterpret_cast<Generator*(*)()>(dlsym(dllHandle, creator.c_str()))();
#endif
    }
     
    CHeaderFile* header = cHeaderBuilder.build();
    generator->create(*os, *header);

    delete header;
    delete generator;
#ifdef _WIN32
    FreeLibrary(dllHandle);
#endif

    return 0;
}
