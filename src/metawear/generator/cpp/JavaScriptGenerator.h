#pragma once

#include "cbinds/Generator.h"
#include "metawear/platform/dllmarker.h"

class JavaScriptGenerator : public mbientlab::cbinds::Generator {
public:
    virtual ~JavaScriptGenerator();

    virtual void create(std::ostream& os, const mbientlab::cbinds::CHeaderFile& header);
};

extern "C" METAWEAR_API mbientlab::cbinds::Generator* createJavaScriptGenerator();
