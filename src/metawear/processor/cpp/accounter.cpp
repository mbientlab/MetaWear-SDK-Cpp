#include <cstdlib>

#include "accounter_private.h"
#include "metawear/processor/accounter.h"
#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/cpp/constant.h"
#include "processor_private_common.h"

using std::calloc;

struct AccounterConfig {
    uint8_t mode:4;
    uint8_t length:2;
    uint8_t :2;
    uint8_t prescale:4;
    uint8_t :4;
};

METAWEAR_API int32_t mbl_mw_dataprocessor_accounter_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    const uint8_t length = 4;   // Fix to 4 byte length for now
    if (source->length() + length + 3 >= BLE_PACKET_SIZE) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    new_processor->channel_size+= length;

    AccounterConfig* config = (AccounterConfig*)calloc(1, sizeof(AccounterConfig));
    config->mode = 0x1;
    config->length = (length - 1);
    config->prescale = 0x3; // This is what the logger uses, so we follow suit for now

    create_processor(source, config, sizeof(AccounterConfig), DataProcessorType::ACCOUNTER, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

uint8_t get_accounter_length(const MblMwDataProcessor* source) {
    return ((AccounterConfig*)source->config)->length + 1;
}

uint8_t get_accounter_prescale(const MblMwDataProcessor* source) {
    return ((AccounterConfig*)source->config)->prescale + 1;
}
