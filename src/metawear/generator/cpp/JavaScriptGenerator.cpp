#include "JavaScriptGenerator.h"
#include "cbinds/Util.h"
#include "common.h"

#include <cctype>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::endl;
using std::function;
using std::isdigit;
using std::isupper;
using std::ostream;
using std::string;
using std::stringstream;
using std::toupper;
using std::unordered_map;
using std::unordered_set;

using namespace mbientlab::cbinds;

static const unordered_map<string, string> FN_TYPE_NAME_MAPPING = {
    { "float" , "Float" },
    { "void" , "Void" },
    { "int64_t" , "Long" }, 
    { "int32_t" , "Int" }, 
    { "int8_t" , "Byte" }, 
    { "uint8_t" , "UByte" }, 
    { "uint16_t" , "UShort" },
    { "uint32_t" , "UInt" },
    { "uint64_t" , "Ulong" }
};

Generator* createJavaScriptGenerator() {
    return new JavaScriptGenerator();
}

JavaScriptGenerator::~JavaScriptGenerator() {

}

void JavaScriptGenerator::create(ostream& os, const CHeaderFile& header) {
    unordered_set<string> exports = {
        "ArrayUByte_6",
        "ArrayUByte_10",
        "ArrayFloat",
        "ArrayUByte_16",
        "ArrayAnonymousDataSignalP",
        "ArrayDataSignalP"
    };
    unordered_map<string, string> typeMapping = {
        { "float" , "ref.types.float" },
        { "void" , "ref.types.void" },
        { "int64_t" , "ref.types.int64" }, 
        { "int32_t" , "ref.types.int32" }, 
        { "int8_t" , "ref.types.int8" }, 
        { "uint8_t" , "ref.types.uint8" }, 
        { "uint16_t" , "ref.types.uint16" },
        { "uint32_t" , "ref.types.uint32" },
        { "uint64_t" , "ref.types.uint64" }      
    };

    os << "// Warning!!!" << endl 
        << "// This JavaScript module is generated from the MetaWear C++ API header files" << endl
        << "// Do not edit" << endl
        << "var ref = require('ref');" << endl
        << "var ffi = require('ffi');" << endl
        << "var Struct = require('ref-struct');" << endl
        << "var ArrayType = require('ref-array');" << endl
        << "var Enum = require('enum');" << endl
        << "var LIBMETAWEAR_PATH = require('./libmetawear-path');" << endl << endl
        << "// TODO: These exist because arrays are not handled perfectly yet" << endl
        << "var ArrayUByte_6 = ArrayType(ref.types.uint8, 6);" << endl
        << "var ArrayFloat = ArrayType(ref.types.float);" << endl
        << "var ArrayUByte_10 = ArrayType(ref.types.uint8, 10);" << endl
        << "var ArrayUByte_16 = ArrayType(ref.types.uint8, 16);" << endl << endl;

    for(auto& it: header.getEnums()) {
        string prefix = createPrefix(it.first);
        string stripType = strip(it.first, "MblMw");
        exports.insert(stripType);
        os << "var " << stripType << " = new Enum({" << endl;
        typeMapping.insert({it.first, stripType});
        bool first = true;
        for(auto& entry: it.second->getEntries()) {
            string stripped = strip(entry.name, prefix);
            if (stripped == entry.name) {
                stripped = strip(entry.name, SHORTENED_PREFIXES.at(it.first));
            }
            if (isdigit(stripped[0])) {
                stripped = "_" + stripped;
            }
            if (!first) {
                os << "," << endl;
            }
            os << "  '" << stripped << "': " << entry.value;
            first = false;
        }
        os << endl << "}, ref.types.int);" << endl; 
        os << stripType << ".alignment = 4;" << endl << endl;
    }

    unordered_set<string> fnPtrs, structs, presetTypeNames;
    auto convertFnTypeName = [&](const Type& type) -> string {
        string stripped(strip(type.getName(), "MblMw"));
        string name= FN_TYPE_NAME_MAPPING.count(stripped) ? FN_TYPE_NAME_MAPPING.at(stripped) : stripped;
        if (type.isPointer()) {
            name+= "P";
        }
        return name;
    };
    auto convertType = [&](const Type& type) -> string {
        auto arraySize = type.getArraySize();
        if (type.isArray()) {
            // only need array of native types for now
            // will need to expand this if we do array of objects or pointers
            stringstream result;
            result << "Array" << FN_TYPE_NAME_MAPPING.at(type.getName());
            if (arraySize != -1) {
                result << "_" << arraySize;
            }
            assert(exports.count(result.str()) && "New Array Type, add to list or generalize this code");
            return result.str();
        } else if (type.isPointer()) {
            auto isFnPtr = dynamic_cast<const FunctionPointer*>(&type) != nullptr;
            if (header.getStructs().count(type.getName())) {
                auto structObj = header.getStructs().at(type.getName());
                return "ref.refType(" + typeMapping.at(structObj->getName()) + ")";
            } else if (type.getName() == "char") {
                return "ref.types.CString";
            } else if (typeMapping.count(type.getName()) && !isFnPtr) {
                return "ref.refType(" + typeMapping.at(type.getName()) + ")";
            } else if (isFnPtr) {
                return typeMapping.at(type.getName());
            }
            return "ref.refType(ref.types.void)";
        } else {
            return typeMapping.at(type.getName());
        }        
    };
    function<void (const Struct*)> printStruct;
    function<void (const FunctionPointer*)> printFunctionPtr = [&](const FunctionPointer* fnPointer) -> void {
        if (fnPtrs.count(fnPointer->getName())) {
            return;
        }

        string mappedName("Fn" + convertFnTypeName(*(fnPointer->getReturnType())));
        for(auto& param: fnPointer->getParameters()) {
            auto& type = param->getType();

            if (header.getStructs().count(type.getName())) {
                printStruct(header.getStructs().at(type.getName()));
            }

            mappedName+= "_" + convertFnTypeName(type);
        }

        typeMapping.insert({fnPointer->getName(), mappedName});
        if (presetTypeNames.count(mappedName)) {
            return;
        }
        presetTypeNames.insert(mappedName);

        exports.insert(mappedName);
        os << "var " << mappedName << " = ffi.Function(";
        os << convertType(*fnPointer->getReturnType()) << ", [";
        bool first = true;
        for(auto& param: fnPointer->getParameters()) {
            if (!first) {
                os << ", ";
            }
            os << convertType(param->getType());
            first = false;
        }
        os << "]);" << endl;
        fnPtrs.insert(fnPointer->getName());
    };
    printStruct = [&](const Struct* structObj) -> void {
        if (structs.count(structObj->getName())) {
            return;
        }

        if (structObj->isForwardDecl()) {
            string strippedName(strip(structObj->getName(), "MblMw"));
            exports.insert(strippedName);
            typeMapping.insert({structObj->getName(), strippedName});
            os << "var " << strippedName << " = ref.types.void;" << endl;
        } else {
            for(auto& field: structObj->getFields()) {
                const FunctionPointer* fnPtr;
                if ((fnPtr = dynamic_cast<const FunctionPointer*>(&field->getType())) != nullptr) {
                    printFunctionPtr(fnPtr);
                }
            }

            string strippedName(strip(structObj->getName(), "MblMw"));
            typeMapping.insert({structObj->getName(), strippedName});

            bool first = true;
            exports.insert(strippedName);
            os << "var " << strippedName << " = Struct({" << endl;
            for(auto field: structObj->getFields()) {
                if (!first) {
                    os << "," << endl;
                }
                os << "  '" << field->getName() << "': " << convertType(field->getType());
                first = false;
            }
            os << endl << "});" << endl << endl;
        }
        structs.insert(structObj->getName());
    };

    for(auto it: header.getFunctionPointers()) {
        printFunctionPtr(it.second);
    }

    for(auto it: header.getStructs()) {
        printStruct(it.second);
    }

    exports.insert("Const");
    os << "function Const() {" << endl << "}" << endl; 
    for(auto it: header.getConstants()) {
        if (header.getStructs().count(it.second->getType()->getName())) {
            auto structObj = header.getStructs().at(it.second->getType()->getName());
            os << "Const." << strip(it.second->getName(), "MBL_MW_") << " = new " << typeMapping.at(it.second->getType()->getName()) << "({";

            bool first = true;
            for(size_t i = 0 ; i < structObj->getFields().size(); i++) {
                if (!first) {
                    os << ", ";
                }
                os << structObj->getFields()[i]->getName() << ": " << it.second->getValueComponents()[i];
                first = false;
            }
            os << "})" << endl;
        } else {
            os << "Const." << strip(it.second->getName(), "MBL_MW_") << " = " << it.second->getValue() << ";" << endl;
        }
    }
    os << endl;

    exports.insert("Lib");
    bool firstFunction = true;
    os << "// TODO: This line exisits because the generator doesn't understand array of pointers" << endl;
    os << "var ArrayAnonymousDataSignalP = ArrayType(ref.refType(AnonymousDataSignal));" << endl;
    os << "FnVoid_VoidP_MetaWearBoardP_AnonymousDataSignalP_UInt = ffi.Function(ref.types.void, [ref.refType(ref.types.void), ref.refType(MetaWearBoard), ArrayAnonymousDataSignalP, ref.types.uint32]);" << endl;
    os << "var ArrayDataSignalP = ArrayType(ref.refType(DataSignal));" << endl;
    os << "var Lib = ffi.Library(LIBMETAWEAR_PATH, {" << endl;
    for(auto it: header.getFunctions()) {
        if (!firstFunction) {
            os << "," << endl << endl;
        }
        os << it.second->getComment() << endl;
        os << "  '" << it.first << "': [" << convertType(*(it.second->getReturnType())) << ", [";

        if (it.first == "mbl_mw_dataprocessor_fuser_create") {
            // hardcode array type definition for fueser
            // need to generalize array bindings for objects and pointers
            os << "ref.refType(DataSignal), ArrayDataSignalP, ref.types.uint32, ref.refType(ref.types.void), FnVoid_VoidP_DataProcessorP]]";
        } else {
            bool first = true;
            for(auto param: it.second->getParameters()) {
                if (!first) {
                    os << ", ";
                }
                os << convertType(param->getType());
                first = false;
            }
            os << "]]";
        }
        firstFunction = false;
    }
    os << endl << "});" << endl << endl;

    bool first = true;
    os << "module.exports = {" << endl;
    for(auto& expor: exports) {
        if (!first) {
            os << "," << endl;
        }
        os << "  " << expor << ": " << expor;
        first = false;
    }
    os << endl << "};" << endl;
}
