#include "cbinds/Generator.h"

namespace mbientlab {
namespace cbinds {

class DummyGenerator : public Generator {
public:
    virtual ~DummyGenerator();

    virtual void create(std::ostream& os, const CHeaderFile& header);
};

}
}