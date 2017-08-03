#include <cstdlib>

#include "packer_private.h"
#include "metawear/processor/packer.h"
#include "processor_private_common.h"

using std::calloc;

struct PackerConfig {
    uint8_t length:5;
    uint8_t :3;
    uint8_t count:5;
    uint8_t :3;
};

METAWEAR_API int32_t mbl_mw_dataprocessor_packer_create(MblMwDataSignal *source, uint8_t count, MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);

    PackerConfig* config = (PackerConfig*)calloc(1, sizeof(PackerConfig));
    config->length = source->length() - 1;
    config->count = count - 1;

    create_processor(source, config, sizeof(PackerConfig), DataProcessorType::PACKER, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

uint8_t get_packer_length(const MblMwDataProcessor* source) {
    return ((PackerConfig*)source->config)->length + 1;
}

uint8_t get_packer_count(const MblMwDataProcessor* source) {
    return ((PackerConfig*)source->config)->count + 1;
}
