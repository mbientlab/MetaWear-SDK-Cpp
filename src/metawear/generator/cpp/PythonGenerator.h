#pragma once

#include "cbinds/Generator.h"
#include "metawear/platform/dllmarker.h"

class PythonGenerator : public mbientlab::cbinds::Generator {
public:
    virtual ~PythonGenerator();

    virtual void create(std::ostream& os, const mbientlab::cbinds::CHeaderFile& header);
};

extern "C" METAWEAR_API mbientlab::cbinds::Generator* createPythonGenerator();