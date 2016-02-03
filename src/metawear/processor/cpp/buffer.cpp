#include "metawear/processor/buffer.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "processor_private_common.h"

#include <cstdlib>

using std::malloc;

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
    create_processor(source, config, sizeof(BufferConfig), DataProcessorType::BUFFER, new_processor, processor_created);

    new_processor->state = create_processor_state_signal(new_processor, new_processor->interpreter);
    return MBL_MW_STATUS_OK;
}
