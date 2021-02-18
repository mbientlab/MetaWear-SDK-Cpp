#pragma once

#include "CHeaderFile.h"

#include <iostream>

namespace mbientlab {
namespace cbinds {

class Generator {
public:
    virtual ~Generator() { }

    virtual void create(std::ostream& os, const CHeaderFile& header) = 0;
};

}
}