#include "PythonGenerator.h"
#include "cbinds/Util.h"
#include "common.h"

#include <cctype>
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

Generator* createPythonGenerator() {
    return new PythonGenerator();
}

PythonGenerator::~PythonGenerator() {

}

void PythonGenerator::create(ostream& os, const CHeaderFile& header) {
    unordered_map<string, string> typeMapping = {
        { "float" , "c_float" },
        { "void" , "None" },
        { "int64_t" , "c_longlong" }, 
        { "int32_t" , "c_int" }, 
        { "int8_t" , "c_byte" }, 
        { "uint8_t" , "c_ubyte" }, 
        { "uint16_t" , "c_ushort" },
        { "uint32_t" , "c_uint" },
        { "uint64_t" , "c_ulonglong" }
    };

    os << "# Warning!!!" << endl 
        << "# This python module is generated from the MetaWear C++ API header files" << endl
        << "# Do not edit" << endl
        << "from ctypes import * " << endl 
        << "import sys" << endl
        << "import copy" << endl
        << endl;
    
    os << "if sys.version_info[0] == 2:" << endl
        << "    range = xrange" << endl << endl
        << "def array_ubyte_eq(a, a_len, b, b_len):" << endl
        << "    if (a_len != b_len):" << endl
        << "        return False" << endl
        << "    for i in range(a_len):" << endl
        << "        if (a[i] != b[i]):" << endl
        << "            return False" << endl
        << "    return True" << endl
        << endl;

    os << "def array_ubyte_to_hex_string(a, a_len):" << endl
        << "    if (a_len == 0):" << endl
        << "        return \"[]\"" << endl
        << "    string = \"[\" + \"0x%02x\" % (a[0])" << endl
        << "    for i in range(a_len - 1):" << endl
        << "        string+= \", \" + \"0x%02x\" % (a[i + 1])" << endl
        << "    string+= \"]\"" << endl
        << "    return string" << endl
        << endl;

    os << "def array_ubyte_deep_copy(a, a_len):" << endl
        << "    ptr = cast(a, POINTER(c_ubyte * a_len))" << endl
        << "    return cast(copy.deepcopy(ptr.contents), POINTER(c_ubyte))" << endl
        << endl;
        
    os << "def module_info_eq(this, that):" << endl
        << "    return (this.name == that.name and array_ubyte_eq(this.extra, this.extra_len, that.extra, that.extra_len) and this.present == that.present and this.implementation == that.implementation and this.revision == that.revision)" << endl
        << endl;

    os << "def module_info_repr(this):" << endl
        << "    return \"{name : %s, extra : %s, extra_len : %d, present : %d, implementation : %d, revision : %d}\" % (this.name, array_ubyte_to_hex_string(this.extra, this.extra_len), this.extra_len, this.present, this.implementation, this.revision)" << endl
        << endl;

    os << "def module_info_deepcopy(this, memo):" << endl
        << "    return ModuleInfo(name = this.name, extra = array_ubyte_deep_copy(this.extra, this.extra_len), extra_len = this.extra_len, present = this.present, implementation = this.implementation, revision = this.revision)" << endl
        <<endl;

    os << "# taken from https://www.python.org/dev/peps/pep-0485/#proposed-implementation" << endl
        << "def is_close(fst, snd):" << endl
        << "    return abs(fst - snd) <= max( 0.001 * max(abs(fst), abs(snd)), 0.001 )" << endl << endl;

    for(auto& it: header.getEnums()) {
        string prefix = createPrefix(it.first);
        os << "class " << strip(it.first, "MblMw") << ":" << endl;
        typeMapping.insert({it.first, "c_int"});
        for(auto& entry: it.second->getEntries()) {
            string stripped = strip(entry.name, prefix);
            if (stripped == entry.name) {
                stripped = strip(entry.name, SHORTENED_PREFIXES.at(it.first));
            }
            if (isdigit(stripped[0])) {
                stripped = "_" + stripped;
            }
            os << "    " << stripped << " = " << entry.value << endl;
        }
        os << endl;
    }

    unordered_set<string> fnPtrs, structs, presetTypeNames;
    auto convertFnTypeName = [&](const Type& type) -> string {
        if (header.getStructs().count(type.getName()) && header.getStructs().at(type.getName())->isForwardDecl() && type.isPointer()) {
            return "VoidP";
        }

        string stripped(strip(type.getName(), "MblMw"));
        string name= FN_TYPE_NAME_MAPPING.count(stripped) ? FN_TYPE_NAME_MAPPING.at(stripped) : stripped;
        if (type.isPointer()) {
            name+= "P";
        }
        return name;
    };
    auto convertType = [&](const Type& type) -> string {
        auto arraySize = type.getArraySize();
        if (type.isArray() && arraySize != -1) {
            // only need array of native types for now
            // will need to expand this if we do array of objects or pointers
            stringstream result;
            result << "(" << typeMapping.at(type.getName()) << " * " << arraySize << ")";
            return result.str();
        } else if (type.isPointer() || (type.isArray() && arraySize == -1)) {
            auto isFnPtr = dynamic_cast<const FunctionPointer*>(&type) != nullptr;
            if (header.getStructs().count(type.getName())) {
                auto structObj = header.getStructs().at(type.getName());
                if (header.getStructs().at(type.getName())->isForwardDecl()) {
                    return "c_void_p";
                } else {
                    return "POINTER(" + typeMapping.at(structObj->getName()) + ")";
                }
            } else if (type.getName() == "char") {
                return "c_char_p";
            } else if (typeMapping.count(type.getName()) && !isFnPtr) {
                return type.getName() == "void" ? "c_void_p" : "POINTER(" + typeMapping.at(type.getName()) + ")";
            } else if (isFnPtr) {
                return typeMapping.at(type.getName());
            }
            return "c_void_p";
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

        os << mappedName << " = CFUNCTYPE(";
        os << convertType(*fnPointer->getReturnType());
        for(auto& param: fnPointer->getParameters()) {
            os << ", " << convertType(param->getType());
        }
        os << ")" << endl;
        fnPtrs.insert(fnPointer->getName());
    };
    printStruct = [&](const Struct* structObj) -> void {
        if (structs.count(structObj->getName())) {
            return;
        }

        if (structObj->isForwardDecl()) {
            typeMapping.insert({structObj->getName(), "c_void_p"});
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
            string eqLine, strFormat, strElem, deepcopyLine;
            os << "class " << strippedName << "(Structure):" << endl 
                << "    _fields_ = [" << endl;
            for(auto field: structObj->getFields()) {
                if (!first) {
                    os << "," << endl;
                    strFormat+= ", ";
                    strElem+= ", ";
                    eqLine+= " and ";
                    deepcopyLine+= ", ";
                }
                os << "        (\"" << field->getName() << "\" , " << convertType(field->getType()) << ")";

                strFormat+= field->getName() + " : ";
                auto fieldType = field->getType().getName();
                if (fieldType == "float") {
                    strFormat+= "%.3f";
                    eqLine+= "is_close(self." + field->getName() + ", other." + field->getName() + ")";
                    strElem+= "self." + field->getName();
                } else if (fieldType == "char" && field->getType().isPointer()) {
                    strFormat+= "%s";
                    eqLine+= "self." + field->getName() + " == " + "other." + field->getName();
                    strElem+= "self." + field->getName();
                } else if (field->getType().isArray() && field->getType().getArraySize() != -1) {
                    strFormat+= "%s";
                    {
                        stringstream buffer;
                        buffer << "array_ubyte_eq(self." << field->getName() << ", " << field->getType().getArraySize() 
                                << ", other." << field->getName() << ", " << field->getType().getArraySize() << ")";
                        eqLine+= buffer.str();
                    }
                    {
                        stringstream buffer;
                        buffer << "array_ubyte_to_hex_string(self." << field->getName() << ", " << field->getType().getArraySize() << ")";
                        strElem+= buffer.str();
                    }
                } else {
                    strFormat+= "%d";
                    eqLine+= "self." + field->getName() + " == " + "other." + field->getName();
                    strElem+= "self." + field->getName();
                }
                
                if (field->getType().isArray() && field->getType().getArraySize() != -1) {
                    deepcopyLine+= field->getName() + " = copy.deepcopy(self." + field->getName() + ")";
                } else {
                    deepcopyLine+= field->getName() + " = self." + field->getName();
                }

                first = false;
            }
            os << endl << "    ]" << endl << endl;

            os << "    def __neq__(self, other):" << endl
                << "        return not self.__eq__(other)" << endl << endl;

            if (structObj->getName() == "MblMwModuleInfo") {
                os << "    def __eq__(self, other):" << endl
                    << "        return module_info_eq(self, other)" << endl << endl;

                os << "    def __repr__(self):" << endl
                    << "        return module_info_repr(self)" << endl << endl;

                os << "    def __deepcopy__(self, memo):" << endl
                    << "        return module_info_deepcopy(self, memo)" << endl << endl;
            } else {
                os << "    def __eq__(self, other):" << endl
                    << "        return (" << eqLine << ")" << endl << endl;

                os << "    def __repr__(self):" << endl
                    << "        return \"{" << strFormat << "}\" % (" << strElem << ")" << endl << endl;

                os << "    def __deepcopy__(self, memo):" << endl
                    << "        return " << strippedName << "(" << deepcopyLine << ")" << endl << endl;
            }
        }
        structs.insert(structObj->getName());
    };

    for(auto it: header.getFunctionPointers()) {
        printFunctionPtr(it.second);
    }

    for(auto it: header.getStructs()) {
        printStruct(it.second);
    }

    os << "class Const:" << endl;
    for(auto it: header.getConstants()) {
        if (header.getStructs().count(it.second->getType()->getName())) {
            auto structObj = header.getStructs().at(it.second->getType()->getName());
            os << "    " << strip(it.second->getName(), "MBL_MW_") << " = " << typeMapping.at(it.second->getType()->getName()) << "(";

            bool first = true;
            for(size_t i = 0 ; i < structObj->getFields().size(); i++) {
                if (!first) {
                    os << ", ";
                }
                os << structObj->getFields()[i]->getName() << " = " << it.second->getValueComponents()[i];
                first = false;
            }
            os << ")" << endl;
        } else {
            os << "    " << strip(it.second->getName(), "MBL_MW_") << " = " << it.second->getValue()  << endl;
        }
    }
    os << endl;

    os << "def init_libmetawear(libmetawear):" << endl;
    for(auto it: header.getFunctions()) {
        os << "    libmetawear." << it.first << ".restype = " << convertType(*(it.second->getReturnType())) << endl;

        os << "    libmetawear." << it.first << ".argtypes = [";
        bool first = true;
        for(auto param: it.second->getParameters()) {
            if (!first) {
                os << ", ";
            }
            os << convertType(param->getType());
            first = false;
        }

        os << "]" << endl << endl;
    }
}
