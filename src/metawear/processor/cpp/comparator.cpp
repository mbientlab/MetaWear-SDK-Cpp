#include "dataprocessor_private.h"

#include "metawear/core/cpp/metawearboard_def.h"
#include "metawear/core/status.h"
#include "metawear/processor/comparator.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;
using std::memset;
using std::uint8_t;

struct ComparatorConfig {
    uint8_t is_signed;
    uint8_t operation;
    uint8_t:8;
    uint8_t reference[4];
};

static inline void create_comprator(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, uint8_t is_signed,
        MblMwFnDataProcessor processor_created) {
    ComparatorConfig *config = (ComparatorConfig*) malloc(sizeof(ComparatorConfig));
    *((uint8_t*) config + 2)= 0;
    config->is_signed= is_signed;
    config->operation= op;

    int32_t scaled_reference= (int32_t) source->number_to_firmware(source, reference);
    memcpy(((uint8_t*)(config)) + 3, &scaled_reference, sizeof(scaled_reference));

    create_processor(source, config, sizeof(ComparatorConfig), DataProcessorType::COMPARATOR, new MblMwDataProcessor(*source), 
            processor_created);
}

void mbl_mw_dataprocessor_create_comparator(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created) {
    if (source->is_signed) {
        mbl_mw_dataprocessor_create_comparator_signed(source, op, reference, processor_created);
    } else {
        mbl_mw_dataprocessor_create_comparator_unsigned(source, op, reference, processor_created);
    }
}

void mbl_mw_dataprocessor_create_comparator_signed(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created) {
    create_comprator(source, op, reference, 1, processor_created);
}

void mbl_mw_dataprocessor_create_comparator_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, 
        MblMwFnDataProcessor processor_created) {
    create_comprator(source, op, reference, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float reference) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        int32_t scaled_reference= (int32_t) comparator->number_to_firmware(comparator, reference);
        memcpy(((uint8_t*)(comparator->config)) + 3, &scaled_reference, sizeof(scaled_reference));
        ((ComparatorConfig*) comparator->config)->operation= op;

        modify_processor_configuration(comparator, sizeof(ComparatorConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_comparator_modify_signal(MblMwDataProcessor *comparator, MblMwComparatorOperation op, 
        MblMwDataSignal *reference_signal) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        ComparatorConfig* current_config= (ComparatorConfig*) comparator->config;

        memset(((uint8_t*)(current_config)) + 3, 0, sizeof(current_config->reference));
        current_config->operation= op;

        EventDataParameter signal_data_token= {reference_signal->length(), reference_signal->offset, 5};
        comparator->owner->data_token= &signal_data_token;

        modify_processor_configuration(comparator, sizeof(ComparatorConfig));
        comparator->owner->data_token= nullptr;

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
