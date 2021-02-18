#include "Builder.h"
#include "cbinds/FunctionPointer.h"

using std::int32_t;
using std::string;
using std::unordered_map;
using std::vector;

#define SET_COMMENT if (comment.data) {\
    this->comment = clang_getCString(comment);\
    clang_disposeString(comment);\
}\
return *this

namespace mbientlab {
namespace cbinds {
namespace builder {

static string extractType(const string& typeString, bool isPointer, bool isArray) {
    size_t last = isArray ? typeString.find("[") - 1 : (isPointer ? typeString.find("*") - 1 : typeString.length());
    size_t first = typeString.find(" ");
    string extracted = first == last ? typeString.substr(0, last) : typeString.substr(first + 1, last - (first + 1) + (first == string::npos ? 1 : 0));
    return extracted;
}

static string extractType(const CXString& tokenizedName, bool isPointer, bool isArray) {
    string result(extractType(clang_getCString(tokenizedName), isPointer, isArray));
    clang_disposeString(tokenizedName);

    return result;
}

class TypeInner : public Type {
public:
    TypeInner(const CXString& name, bool isPointer, bool isArray, long long arraySize) : 
            TypeInner(clang_getCString(name), isPointer, isArray, arraySize) {
        clang_disposeString(name);
    }

    TypeInner(const string& name, bool isPointer, bool isArray, long long arraySize) : 
            name(name), arraySize(arraySize), pointer(isPointer), array(isArray) {
    }

    ~TypeInner() { }

    virtual const string& getName() const {
        return name;
    }
    virtual bool isPointer() const {
        return pointer;
    }
    virtual bool isArray() const {
        return array;
    }
    virtual long long getArraySize() const {
        return arraySize;
    }

private:
    string name;
    long long arraySize;
    bool pointer, array;
};

class FunctionPointerInner : public FunctionPointer {
public:
    FunctionPointerInner(const string& comment, const string& name, const vector<Variable*>& parameters, const Type* retType) : 
            name(name), parameters(parameters), retType(retType) { }
    ~FunctionPointerInner() { }

    virtual bool isPointer() const {
        return true;
    }
    virtual bool isArray() const {
        return false;
    }
    virtual long long getArraySize() const {
        return -1;
    }
    virtual const string& getName() const {
        return name;
    }
    virtual const vector<Variable*>& getParameters() const {
        return parameters;
    }
    virtual const Type* getReturnType() const {
        return retType;
    }
    virtual const std::string& getComment() const {
        return comment;
    }

private:
    string comment;
    string name;
    const vector<Variable*> parameters;
    const Type* retType;
};

TypeBuilder::TypeBuilder(const std::string& name) : 
        arraySize(-1), name(name), isFnPointer(false), isPointer(false), isArray(false), retType(nullptr) { }

TypeBuilder::TypeBuilder(const CXString& name) : TypeBuilder(clang_getCString(name)) {
    clang_disposeString(name);
}

Type* TypeBuilder::build() {
    if (isFnPointer) {
        return new FunctionPointerInner(comment, name, parameters, retType == nullptr? new TypeInner("void", false, false, -1) : retType);
     }
     return new TypeInner(extractType(name, isPointer, isArray), isPointer, isArray, arraySize);
}

TypeBuilder& TypeBuilder::setFnPointer() {
    isFnPointer = true;
    setPointer();

    return *this;
}

TypeBuilder& TypeBuilder::setPointer() {
    isPointer = true;
    return *this;
}

TypeBuilder& TypeBuilder::setArray(int32_t size) {
    isArray = true;
    arraySize = size;
    return *this;
}

TypeBuilder& TypeBuilder::addParameter(Variable* var) {
    parameters.push_back(var);
    return *this;
}

TypeBuilder& TypeBuilder::setReturnType(const CXString& name, bool isPointer) {
    retType = new TypeInner(name, isPointer, false, -1);
    return *this;
}

TypeBuilder& TypeBuilder::setReturnType(const Type* type) {
    retType = type;
    return *this;
}

TypeBuilder& TypeBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

class VariableInner : public Variable {
public:
    VariableInner(const string& comment, const CXString& name, const Type* type) : 
            comment(comment), name(clang_getCString(name)) , type(type) {
        clang_disposeString(name);
    }
    VariableInner(const string& comment, const string& name, const Type* type) : 
            comment(comment), name(name), type(type) { }

    ~VariableInner() {
        delete type;
    }

    virtual const std::string& getComment() const {
        return comment;
    }
    virtual const std::string& getName() const {
        return name;
    }
    virtual const Type& getType() const {
        return *type;
    };

private:
    string comment;
    string name;
    const Type* type;
};

VariableBuilder::VariableBuilder(const CXString& name) : name(clang_getCString(name)) {
    clang_disposeString(name);
}

VariableBuilder& VariableBuilder::setType(const Type& type) {
    this->type = &type;
    return *this;
}

VariableBuilder& VariableBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

Variable* VariableBuilder::build() {
    return new VariableInner(comment, name, type);
}

class FunctionInner : public Function {
public:
    FunctionInner(const string& comment, const string& name, const vector<Variable*>& parameters, const Type* retType) : 
            comment(comment), name(name), parameters(parameters), retType(retType) { }
    ~FunctionInner() {
        delete retType;
        for(auto it: parameters) {
            delete it;
        }
    }

    virtual const string& getName() const {
        return name;
    }
    virtual const vector<Variable*>& getParameters() const {
        return parameters;
    }
    virtual const Type* getReturnType() const {
        return retType;
    }
    virtual const std::string& getComment() const {
        return comment;
    }
private:
    string comment;
    string name;
    const vector<Variable*> parameters;
    const Type* retType;
};

FunctionBuilder::FunctionBuilder(const CXString& name) : name(clang_getCString(name)), retType(nullptr)  {
    clang_disposeString(name);
}

FunctionBuilder& FunctionBuilder::addParameter(const CXString& name, const CXString& typeName, bool isPointer, bool isArray, long long arraySize) {
    VariableBuilder builder(name);

    Type* type = new TypeInner(extractType(typeName, isPointer, isArray), isPointer, isArray, arraySize);
    builder.setType(*type);
    parameters.push_back(builder.build());

    return *this;
}

FunctionBuilder& FunctionBuilder::setReturnType(const CXString& name, bool isPointer) {
    retType = new TypeInner(extractType(name, isPointer, false), isPointer, false, -1);
    return *this;
}

FunctionBuilder& FunctionBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

Function* FunctionBuilder::build() {
    return new FunctionInner(comment, name, parameters, retType == nullptr? new TypeInner("void", false, false, -1) : retType);
}

class StructInner : public Struct {
public:
    StructInner(const string& comment, const string& name, const vector<Variable*>& fields, bool forwardDecl) : 
            name(name), fields(fields), forwardDecl(forwardDecl) { }

    ~StructInner() { }

    virtual const string& getName() const {
        return name;
    }
    virtual const vector<Variable*>& getFields() const {
        return fields;
    }
    virtual bool isForwardDecl() const {
        return forwardDecl;
    }
    virtual const std::string& getComment() const {
        return comment;
    }
private:
    string comment;
    string name;
    vector<Variable*> fields;
    bool forwardDecl;
};

StructBuilder::StructBuilder(const CXString& name) : name(clang_getCString(name)), isForwardDecl(false) {
    clang_disposeString(name);
}

StructBuilder& StructBuilder::addField(Variable* field) {
    fields.push_back(field);
    return *this;
}

StructBuilder& StructBuilder::setName(const CXString& name) {
    this->name = clang_getCString(name);
    clang_disposeString(name);
    return *this;
}

StructBuilder& StructBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

StructBuilder& StructBuilder::setForwardDecl() {
    isForwardDecl = true;
    return *this;
}

Struct* StructBuilder::build() {
    return new StructInner(comment, name, fields, isForwardDecl);
}

class EnumInner : public Enum {
public:
    EnumInner(const string& comment, const string& name, const vector<EnumEntry>& entries) : 
            comment(comment), name(name), entries(entries) { };
    ~EnumInner() { }

    virtual const string& getComment() const {
        return comment;
    }
    virtual const string& getName() const {
        return name;
    }
    virtual const vector<EnumEntry>& getEntries() const {
        return entries;
    }
private:
    string comment;
    string name;
    vector<EnumEntry> entries;
};

EnumBuilder::EnumBuilder() {

}

EnumBuilder& EnumBuilder::addEntry(const CXString& comment, const CXString& name, std::int32_t value) {
    if (comment.data) {
        EnumEntry entry = { clang_getCString(comment), clang_getCString(name), value };
        clang_disposeString(comment);
        clang_disposeString(name);
        entries.push_back(entry);

        return *this;
    } else {
        return addEntry(name, value);
    }
}

EnumBuilder& EnumBuilder::addEntry(const CXString& name, int32_t value) {
    EnumEntry entry;
    entry.name = clang_getCString(name);
    clang_disposeString(name);
    entry.value = value;
    entries.push_back(entry);

    return *this;
}

EnumBuilder& EnumBuilder::setName(const CXString& name) {
    this->name = clang_getCString(name);
    clang_disposeString(name);
    return *this;
}

EnumBuilder& EnumBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

Enum* EnumBuilder::build() {
    return new EnumInner(comment, name, entries);
}

class ConstantInner : public Constant {
public:
    ConstantInner(Type* type, const string& comment, const string& name, const string& value, const vector<string>& valueComponents) :
        type(type), comment(comment), name(name), value(value), valueComponents(valueComponents) { }
    virtual ~ConstantInner() {
        delete type;
    }

    virtual const std::string& getName() const {
        return name;
    }
    virtual const Type* getType() const {
        return type;
    }
    virtual const string& getValue() const {
        return value;
    }
    virtual const vector<string>& getValueComponents() const {
        return valueComponents;
    }
    virtual const string& getComment() const {
        return comment;
    }
private:
    Type* type;
    string comment;
    string name;
    string value;
    vector<string> valueComponents;
};

ConstBuilder::ConstBuilder(const CXString& name, Type* type) : type(type), name(clang_getCString(name)) {
    clang_disposeString(name);
}

ConstBuilder& ConstBuilder::setValue(const string& value) {
    this->value = value;
    return *this;
}

ConstBuilder& ConstBuilder::addValueComponent(const string& component) {
    this->valueComponents.push_back(component);
    return *this;
}

Constant* ConstBuilder::build() {
    return new ConstantInner(type, comment, name, value, valueComponents);
}

ConstBuilder& ConstBuilder::setComment(const CXString& comment) {
    SET_COMMENT;
}

class CHeaderFileInner : public CHeaderFile {
public:
    CHeaderFileInner(const unordered_map<string, Enum*>& enums, const unordered_map<string, Function*>& functions,
        const unordered_map<string, Constant*>& constants, const unordered_map<string, Struct*>& structs, 
        const unordered_map<string, FunctionPointer*>& functionPtrs) : 
        enums(enums), functions(functions), functionPtrs(functionPtrs), constants(constants), structs(structs) { }

    virtual ~CHeaderFileInner() {
        for(auto const& it: enums) {
            delete it.second;
        }
        for(auto const& it: functions) {
            delete it.second;
        }
        for(auto const& it: functionPtrs) {
            delete it.second;
        }
        for(auto const& it: constants) {
            delete it.second;
        }
        for(auto const& it: structs) {
            delete it.second;
        }
    }

    virtual const unordered_map<string, Enum*>& getEnums() const {
        return enums;
    }

    virtual const unordered_map<string, Function*>& getFunctions() const {
        return functions;
    }

    virtual const unordered_map<string, Constant*>& getConstants() const {
        return constants;
    }

    virtual const unordered_map<string, Struct*>& getStructs() const {
        return structs;
    }

    virtual const unordered_map<string, FunctionPointer*>& getFunctionPointers() const {
        return functionPtrs;
    }

private:
    unordered_map<string, Enum*> enums;
    unordered_map<string, Function*> functions;
    unordered_map<string, FunctionPointer*> functionPtrs;
    unordered_map<string, Constant*> constants;
    unordered_map<string, Struct*> structs;
};

CHeaderFileBuilder& CHeaderFileBuilder::addConstant(Constant* newConst) {
    constants.insert({newConst->getName(), newConst});
    return *this;
}

CHeaderFileBuilder& CHeaderFileBuilder::addFunction(Function* newFn) {
    functions.insert({newFn->getName(), newFn});
    return *this;
}

CHeaderFileBuilder& CHeaderFileBuilder::addFunctionPtr(FunctionPointer* newFnPtr) {
    functionPtrs.insert({newFnPtr->getName(), newFnPtr});
    return *this;
}

CHeaderFileBuilder& CHeaderFileBuilder::addEnum(Enum* newEnum) {
    enums.insert({newEnum->getName(), newEnum});
    return *this;
}

CHeaderFileBuilder& CHeaderFileBuilder::addStruct(Struct* newStruct) {
    structs.insert({newStruct->getName(), newStruct});
    return *this;
}

CHeaderFile* CHeaderFileBuilder::build() {
    return new CHeaderFileInner(enums, functions, constants, structs, functionPtrs);
}

}
}
}
