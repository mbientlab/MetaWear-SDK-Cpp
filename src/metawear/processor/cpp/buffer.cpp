#include "metawear/processor/buffer.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/version.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::malloc;

const Version BUFFER_PROCESSOR(1, 1, 0);

struct BufferConfig {
    uint8_t length:5;
};

int32_t mbl_mw_dataprocessor_buffer_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    if (source->owner->firmware_revision < BUFFER_PROCESSOR) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    BufferConfig *config= (BufferConfig*) malloc(sizeof(BufferConfig));
    *((uint8_t*) config)= 0;
    config->length= source->length() - 1;

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    create_processor_state_signal(new_processor, new_processor->interpreter);

    create_processor(source, config, sizeof(BufferConfig), DataProcessorType::BUFFER, new_processor, processor_created);
    return MBL_MW_STATUS_OK;
}
