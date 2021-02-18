#pragma once

#include "cbinds/CHeaderFile.h"
#include "cbinds/Enum.h"
#include "cbinds/Function.h"
#include "cbinds/Struct.h"
#include "cbinds/Type.h"
#include "cbinds/Variable.h"

#include <cstdint>
#include <clang-c/Index.h>
#include <string>
#include <vector>

namespace mbientlab {
namespace cbinds {
namespace builder {

class TypeBuilder {
public:
    TypeBuilder(const std::string& name);
    TypeBuilder(const CXString& name);

    TypeBuilder& setFnPointer();
    TypeBuilder& setPointer();
    TypeBuilder& setArray(std::int32_t size);

    TypeBuilder& addParameter(Variable* var);
    TypeBuilder& setReturnType(const CXString& name, bool isPointer);
    TypeBuilder& setReturnType(const Type* type);

    TypeBuilder& setComment(const CXString& comment);

    Type* build();

private:
    std::int32_t arraySize;
    std::string comment;
    std::string name;
    bool isFnPointer, isPointer, isArray;
    std::vector<Variable*> parameters;
    const Type* retType;
};

class VariableBuilder {
public:
    VariableBuilder(const CXString& name);
    VariableBuilder& setType(const Type& type);
    VariableBuilder& setComment(const CXString& comment);
    Variable* build();

private:
    std::string comment;
    std::string name;
    const Type* type;
};

class FunctionBuilder {
public:
    FunctionBuilder(const CXString& name);
    FunctionBuilder& addParameter(const CXString& name, const CXString& typeName, bool isPointer, bool isArray, long long arraySize);
    FunctionBuilder& setReturnType(const CXString& name, bool isPointer);
    FunctionBuilder& setComment(const CXString& comment);
    Function* build();
private:
    std::string comment;
    std::string name;
    std::vector<Variable*> parameters;
    const Type* retType;
};

class StructBuilder {
public:
    StructBuilder();
    StructBuilder(const CXString& name);
    StructBuilder& addField(Variable* field);
    StructBuilder& setName(const CXString& name);
    StructBuilder& setComment(const CXString& comment);
    StructBuilder& setForwardDecl();
    Struct* build();
private:
    std::string comment;
    std::string name;
    std::vector<Variable*> fields;
    bool isForwardDecl;
};

class EnumBuilder {
public:
    EnumBuilder();
    
    EnumBuilder& addEntry(const CXString& name, std::int32_t value);
    EnumBuilder& addEntry(const CXString& comment, const CXString& name, std::int32_t value);
    EnumBuilder& setComment(const CXString& name);
    EnumBuilder& setName(const CXString& name);
    Enum* build();
private:
    std::string comment;
    std::string name;
    std::vector<EnumEntry> entries;
};

class ConstBuilder {
public:
    ConstBuilder(const CXString& name, Type* type);
    ConstBuilder& setValue(const std::string& value);
    ConstBuilder& addValueComponent(const std::string& component);
    ConstBuilder& setComment(const CXString& comment);
    Constant* build();
private:
    Type* type;
    std::string comment;
    std::string name;
    std::string value;
    std::vector<std::string> valueComponents;
};

class CHeaderFileBuilder {
public:
    CHeaderFileBuilder& addFunctionPtr(FunctionPointer* newFnPtr);
    CHeaderFileBuilder& addFunction(Function* newFn);
    CHeaderFileBuilder& addEnum(Enum* newEnum);
    CHeaderFileBuilder& addStruct(Struct* newStruct);
    CHeaderFileBuilder& addConstant(Constant* newConst);
    CHeaderFile* build();
private:
    std::unordered_map<std::string,Constant*> constants;
    std::unordered_map<std::string,Enum*> enums;
    std::unordered_map<std::string,FunctionPointer*> functionPtrs;
    std::unordered_map<std::string,Struct*> structs;
    std::unordered_map<std::string,Function*> functions;
};

}
}
}
