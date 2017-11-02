#include "dataprocessor_config.h"
#include "dataprocessor_private.h"

#include "metawear/core/status.h"
#include "metawear/core/cpp/constant.h"
#include "metawear/core/cpp/metawearboard_def.h"

#include "metawear/processor/accounter.h"
#include "metawear/processor/accumulator.h"
#include "metawear/processor/average.h"
#include "metawear/processor/buffer.h"
#include "metawear/processor/comparator.h"
#include "metawear/processor/counter.h"
#include "metawear/processor/delta.h"
#include "metawear/processor/math.h"
#include "metawear/processor/packer.h"
#include "metawear/processor/passthrough.h"
#include "metawear/processor/pulse.h"
#include "metawear/processor/rms.h"
#include "metawear/processor/rss.h"
#include "metawear/processor/sample.h"
#include "metawear/processor/threshold.h"
#include "metawear/processor/time.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>

using std::malloc;
using std::min;
using std::memset;

uint8_t get_accounter_length(const MblMwDataProcessor* source) {
    return ((AccounterConfig*)source->config)->length + 1;
}

uint8_t get_accounter_prescale(const MblMwDataProcessor* source) {
    return ((AccounterConfig*)source->config)->prescale + 1;
}

int32_t mbl_mw_dataprocessor_accounter_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    const uint8_t length = 4;   // Fix to 4 byte length for now
    if (source->length() + length + 3 >= BLE_PACKET_SIZE) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    AccounterConfig config;
    memset(&config, 0, sizeof(AccounterConfig));
    config.mode = 0x1;
    config.length = (length - 1);
    config.prescale = 0x3; // This is what the logger uses, so we follow suit for now

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::ACCOUNTER), &config), processor_created);
    
    return MBL_MW_STATUS_OK;
}

static int32_t create_accumulator_processor(MblMwDataSignal *source, uint8_t n_channels, uint8_t channel_size, 
        DataProcessorType accum_type, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    AccumulatorConfig config;
    memset(&config, 0, sizeof(AccumulatorConfig));
    config.output = (n_channels * channel_size) - 1;
    config.input= (source->length() - 1);
    config.mode= accum_type == DataProcessorType::ACCUMULATOR ? ACCUMULATOR_SUM : ACCUMULATOR_COUNT;

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(accum_type), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_accumulator_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_accumulator_processor(source, source->n_channels, source->channel_size, DataProcessorType::ACCUMULATOR, 
            processor_created);
}
int32_t mbl_mw_dataprocessor_accumulator_create_size(MblMwDataSignal *source, uint8_t output_size,
        MblMwFnDataProcessor processor_created) {
    return create_accumulator_processor(source, source->n_channels, output_size, DataProcessorType::ACCUMULATOR, processor_created);
}
int32_t mbl_mw_dataprocessor_set_accumulator_state(MblMwDataProcessor *accumulator, float new_running_sum) {
    if (accumulator->type == DataProcessorType::ACCUMULATOR) {
        uint32_t scaled_sum= (uint32_t) number_to_firmware_converters.at(accumulator->converter)(accumulator, new_running_sum);
        set_processor_state(accumulator, &scaled_sum, sizeof(scaled_sum));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_counter_create(MblMwEvent *source, MblMwFnDataProcessor processor_created) {
    return mbl_mw_dataprocessor_counter_create_size(source, 1, processor_created);
}
int32_t mbl_mw_dataprocessor_counter_create_size(MblMwEvent *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    if (MblMwDataSignal* src_signal= dynamic_cast<MblMwDataSignal*>(source)) {
        return create_accumulator_processor(src_signal, 1, size, DataProcessorType::COUNTER, processor_created);
    }

    MblMwDataSignal event_signal(*source);
    return create_accumulator_processor(&event_signal, 1, 1, DataProcessorType::COUNTER, processor_created);
}
int32_t mbl_mw_dataprocessor_counter_set_state(MblMwDataProcessor *counter, uint32_t new_count) {
    if (counter->type == DataProcessorType::COUNTER) {
        set_processor_state(counter, &new_count, sizeof(new_count));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

static int32_t apply_average(MblMwDataSignal *source, bool use_hpf, uint8_t size, MblMwFnDataProcessor processor_created) {
    bool has_hpf = source->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= HPF_REVISION;

    if (!has_hpf && source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    AverageConfig config;
    memset(&config, 0, sizeof(AverageConfig));
    config.output= source->length() - 1;
    config.input= source->length() - 1;
    config.size= size;
    config.count = source->n_channels - 1;

    if (has_hpf && use_hpf) {
        config.mode = USE_HPF;
    }

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::AVERAGE), &config), processor_created);
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_average_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return mbl_mw_dataprocessor_lowpass_create(source, size, processor_created);
}

int32_t mbl_mw_dataprocessor_highpass_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return apply_average(source, true, size, processor_created);
}

int32_t mbl_mw_dataprocessor_lowpass_create(MblMwDataSignal *source, uint8_t size, MblMwFnDataProcessor processor_created) {
    return apply_average(source, false, size, processor_created);
}

int32_t mbl_mw_dataprocessor_average_reset(MblMwDataProcessor *average) {
    if (average->type == DataProcessorType::AVERAGE) {
        set_processor_state(average, nullptr, 0);
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_average_modify_size(MblMwDataProcessor *average, uint8_t size) {
    if (average->type == DataProcessorType::AVERAGE) {
        bool has_hpf = average->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= HPF_REVISION;
        ((AverageConfig*) average->config)->size= size;
        modify_processor_configuration(average, sizeof(AverageConfig) - (has_hpf ? 0 : 1));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

const Version BUFFER_PROCESSOR(1, 1, 0);

int32_t mbl_mw_dataprocessor_buffer_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    if (source->owner->firmware_revision < BUFFER_PROCESSOR) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    BufferConfig config;
    memset(&config, 0, sizeof(BufferConfig));
    config.length= source->length() - 1;

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::BUFFER), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

static inline int32_t create_combiner(MblMwDataSignal *source, DataProcessorType combiner_type,
        MblMwFnDataProcessor processor_created) {
    if (source->n_channels <= 1) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    CombinerConfig config;
    config.output= source->channel_size - 1;
    config.input= source->channel_size - 1;
    config.count= source->n_channels - 1;
    config.is_signed= source->is_signed;
    config.mode= combiner_type == DataProcessorType::RMS ? COMBINER_RMS : COMBINER_RSS;

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(combiner_type), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_rms_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_combiner(source, DataProcessorType::RMS, processor_created);
}

int32_t mbl_mw_dataprocessor_rss_create(MblMwDataSignal *source, MblMwFnDataProcessor processor_created) {
    return create_combiner(source, DataProcessorType::RSS, processor_created);
}

#define SCALE_MULTI_COMP_REFERENCE(cfg, signal, type) for(uint8_t i= 0; i < references_length; i++) {\
    type scaled= (type) number_to_firmware_converters.at(signal->converter)(signal, references[i]);\
    memcpy(cfg.references + offset, &scaled, sizeof(type));\
    offset+= sizeof(type);\
}\

static int32_t create_multi_comparator(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, float references[], 
        uint8_t references_length, uint8_t is_signed, MblMwFnDataProcessor processor_created) {
    if (source->owner->firmware_revision < MULTI_COMPARE || source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    MultiComparatorConfig config;
    config.is_signed= is_signed;
    config.length= source->length() - 1;
    config.operation= op;
    config.mode= mode;

    uint8_t offset= 0;
    switch(source->length()) {
    case 1:
        SCALE_MULTI_COMP_REFERENCE(config, source, int8_t);
        break;
    case 2:
        SCALE_MULTI_COMP_REFERENCE(config, source, int16_t);
        break;
    case 4:
        SCALE_MULTI_COMP_REFERENCE(config, source, int32_t);
        break;
    }

    auto new_processor = MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::COMPARATOR), &config);
    new_processor->config_size = 1 + offset;
    create_processor(source, new_processor, processor_created);

    return MBL_MW_STATUS_OK;
}

static inline int32_t create_comprator(MblMwDataSignal *source, MblMwComparatorOperation op, float reference, uint8_t is_signed, 
    MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    if (source->owner->firmware_revision < MULTI_COMPARE) {
        ComparatorConfig config;
        *((uint8_t*) &config + 2)= 0;
        config.is_signed= is_signed;
        config.operation= op;

        int32_t scaled_reference= (int32_t) number_to_firmware_converters.at(source->converter)(source, reference);
        memcpy(((uint8_t*)(&config)) + 3, &scaled_reference, sizeof(scaled_reference));

        create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::COMPARATOR), &config), processor_created);
        return MBL_MW_STATUS_OK;
    } else {
        return create_multi_comparator(source, op, MBL_MW_COMPARATOR_MODE_ABSOLUTE, &reference, 1, is_signed, processor_created);
    }
}

int32_t mbl_mw_dataprocessor_comparator_create(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    return create_comprator(source, op, reference, source->is_signed ? 1 : 0, processor_created);
}

int32_t  mbl_mw_dataprocessor_comparator_create_signed(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    return create_comprator(source, op, reference, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_comparator_create_unsigned(MblMwDataSignal *source, MblMwComparatorOperation op, float reference,
        MblMwFnDataProcessor processor_created) {
    return create_comprator(source, op, reference, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float reference) {
    if (!(comparator->owner->firmware_revision < MULTI_COMPARE)) {
        return mbl_mw_dataprocessor_multi_comparator_modify(comparator, op, &reference, 1);
    }

    if (comparator->type == DataProcessorType::COMPARATOR) {
        int32_t scaled_reference= (int32_t) number_to_firmware_converters.at(comparator->converter)(comparator, reference);
        memcpy(((uint8_t*)(comparator->config)) + 3, &scaled_reference, sizeof(scaled_reference));
        ((ComparatorConfig*) comparator->config)->operation= op;

        modify_processor_configuration(comparator, sizeof(ComparatorConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_comparator_modify_signal(MblMwDataProcessor *comparator, MblMwComparatorOperation op, MblMwDataSignal *reference_signal) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        if (comparator->owner->firmware_revision < MULTI_COMPARE) {
            ComparatorConfig* current_config= (ComparatorConfig*) comparator->config;

            memset(((uint8_t*)(current_config)) + 3, 0, sizeof(current_config->reference));
            current_config->operation= op;

            EventDataParameter signal_data_token= {reference_signal->length(), reference_signal->offset, 5};

            set_data_token(comparator->owner, &signal_data_token);
            modify_processor_configuration(comparator, sizeof(ComparatorConfig));
            clear_data_token(comparator->owner);

            return MBL_MW_STATUS_OK;
        } else {
            MultiComparatorConfig* config= (MultiComparatorConfig*) malloc(sizeof(MultiComparatorConfig));

            *((uint8_t*) config)= *((uint8_t*) comparator->config);
            config->operation= op;

            uint8_t real_length= min((uint8_t) (config->length + 1), reference_signal->length());
            free(comparator->config);
            comparator->config= config;
            comparator->config_size= 1 + real_length;
            memset(config->references, 0, real_length);

            EventDataParameter signal_data_token= {real_length, reference_signal->offset, 3};

            set_data_token(comparator->owner, &signal_data_token);
            modify_processor_configuration(comparator, comparator->config_size);
            clear_data_token(comparator->owner);

            return MBL_MW_STATUS_OK;
        }
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_multi_comparator_create(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, float references[], 
        uint8_t references_length, MblMwFnDataProcessor processor_created) {
    return create_multi_comparator(source, op, mode, references, references_length, source->is_signed ? 1 : 0, processor_created);
}

int32_t mbl_mw_dataprocessor_multi_comparator_create_signed(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created) {
    return create_multi_comparator(source, op, mode, references, references_length, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_multi_comparator_create_unsigned(MblMwDataSignal* source, MblMwComparatorOperation op, MblMwComparatorMode mode, 
        float references[], uint8_t references_length, MblMwFnDataProcessor processor_created) {
    return create_multi_comparator(source, op, mode, references, references_length, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_multi_comparator_modify(MblMwDataProcessor *comparator, MblMwComparatorOperation op, float references[], 
        uint8_t references_length) {
    if (comparator->type == DataProcessorType::COMPARATOR) {
        MultiComparatorConfig* config= (MultiComparatorConfig*) malloc(sizeof(MultiComparatorConfig));

        *((uint8_t*) config)= *((uint8_t*) comparator->config);
        config->operation= op;

        uint8_t offset= 0;
        switch(config->length) {
        case 0:
            SCALE_MULTI_COMP_REFERENCE((*config), comparator, int8_t);
            break;
        case 1:
            SCALE_MULTI_COMP_REFERENCE((*config), comparator, int16_t);
            break;
        case 3:
            SCALE_MULTI_COMP_REFERENCE((*config), comparator, int32_t);
            break;
        }

        free(comparator->config);
        comparator->config= config;
        comparator->config_size= 1 + offset;

        modify_processor_configuration(comparator, comparator->config_size);
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_delta_create(MblMwDataSignal *source, MblMwDeltaMode mode, float magnitude,
        MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    uint32_t scaled_magnitude= (uint32_t) number_to_firmware_converters.at(source->converter)(source, magnitude);

    DeltaConfig config;
    *((uint8_t*) &config)= 0;
    config.length= source->length() - 1;
    config.is_signed= source->is_signed;
    config.mode= mode;
    memcpy(((uint8_t*)(&config)) + 1, &scaled_magnitude, sizeof(scaled_magnitude));

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::DELTA), &config), processor_created);
    
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_delta_set_reference(MblMwDataProcessor *delta, float previous_value) {
    if (delta->type == DataProcessorType::DELTA) {
        int32_t scaled_previous_value= (int32_t) number_to_firmware_converters.at(delta->converter)(delta, previous_value);
        set_processor_state(delta, &scaled_previous_value, sizeof(scaled_previous_value));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_delta_modify_magnitude(MblMwDataProcessor *delta, float magnitude) {
    if (delta->type == DataProcessorType::DELTA) {
        uint32_t scaled_magnitude= (uint32_t) number_to_firmware_converters.at(delta->converter)(delta, magnitude);
        memcpy(((uint8_t*)(delta->config)) + 1, &scaled_magnitude, sizeof(scaled_magnitude));

        modify_processor_configuration(delta, sizeof(DeltaConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

static int32_t create_math(MblMwDataSignal *source, MblMwMathOperation op, float rhs, uint8_t is_signed,
        MblMwFnDataProcessor processor_created) {
    if ((source->n_channels > 1 && source->owner->firmware_revision < MULTI_CHANNEL_MATH) || 
            (op == MBL_MW_MATH_OP_CONSTANT && source->owner->firmware_revision < CONST_MATH_OP)) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    int32_t scaled_rhs;
    switch (op) {
    case MBL_MW_MATH_OP_ADD:
    case MBL_MW_MATH_OP_SUBTRACT:
    case MBL_MW_MATH_OP_MODULUS:
        scaled_rhs= (int32_t) number_to_firmware_converters.at(source->converter)(source, rhs);
        break;
    default:
        scaled_rhs= (int32_t) rhs;
        break;
    }

    MathConfig config;
    *((uint8_t*) &config)= 0;
    config.input= source->channel_size - 1;
    config.is_signed= is_signed;
    config.operation= op;
    config.n_channels= source->n_channels - 1;

    memcpy(((uint8_t*)(&config)) + 2, &scaled_rhs, sizeof(scaled_rhs));
    
    auto new_processor = MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::MATH), &config);
    ((MathConfig*) new_processor->config)->output= new_processor->channel_size - 1;
    create_processor(source, new_processor, processor_created);
    
    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_math_create(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    if (source->is_signed) {
        return mbl_mw_dataprocessor_math_create_signed(source, op, rhs, processor_created);
    } else {
        return mbl_mw_dataprocessor_math_create_unsigned(source, op, rhs, processor_created);
    }
}

int32_t mbl_mw_dataprocessor_math_create_signed(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    return create_math(source, op, rhs, 1, processor_created);
}

int32_t mbl_mw_dataprocessor_math_create_unsigned(MblMwDataSignal *source, MblMwMathOperation op, float rhs,
        MblMwFnDataProcessor processor_created) {
    return create_math(source, op, rhs, 0, processor_created);
}

int32_t mbl_mw_dataprocessor_math_modify_rhs(MblMwDataProcessor *math, float rhs) {
    if (math->type == DataProcessorType::MATH) {
        MathConfig* current_config= (MathConfig*) math->config;

        int32_t scaled_rhs;
        switch (current_config->operation) {
        case MBL_MW_MATH_OP_ADD:
        case MBL_MW_MATH_OP_SUBTRACT:
        case MBL_MW_MATH_OP_MODULUS:
            scaled_rhs= (int32_t) number_to_firmware_converters.at(math->converter)(math, rhs);
            break;
        default:
            scaled_rhs= (int32_t) rhs;
            break;
        }
        memcpy(((uint8_t*)(current_config)) + 2, &scaled_rhs, sizeof(scaled_rhs));

        modify_processor_configuration(math, sizeof(MathConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_math_modify_rhs_signal(MblMwDataProcessor *math, MblMwDataSignal* rhs_signal) {
    if (math->type == DataProcessorType::MATH) {
        MathConfig *current_config= (MathConfig*) math->config;

        memset(((uint8_t*)(current_config)) + 2, 0, sizeof(current_config->rhs));

        EventDataParameter signal_data_token= {rhs_signal->length(), rhs_signal->offset, 4};

        set_data_token(math->owner, &signal_data_token);
        modify_processor_configuration(math, sizeof(MathConfig));
        clear_data_token(math->owner);
        
        return MBL_MW_STATUS_OK;
    }

    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

uint8_t get_packer_length(const MblMwDataProcessor* source) {
    return ((PackerConfig*)source->config)->length + 1;
}

uint8_t get_packer_count(const MblMwDataProcessor* source) {
    return ((PackerConfig*)source->config)->count + 1;
}

int32_t mbl_mw_dataprocessor_packer_create(MblMwDataSignal *source, uint8_t count, MblMwFnDataProcessor processor_created) {
    if (source->length() * count + 3 >= BLE_PACKET_SIZE) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    PackerConfig config;
    memset(&config, 0, sizeof(PackerConfig));
    config.length = source->length() - 1;
    config.count = count - 1;

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::PACKER), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_passthrough_create(MblMwDataSignal *source, MblMwPassthroughMode mode, uint16_t count,
        MblMwFnDataProcessor processor_created) {
    PassthroughConfig config;
    config.mode= mode;
    memcpy(((uint8_t*)(&config)) + 1, &count, sizeof(count));
    
    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::PASSTHROUGH), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_passthrough_set_count(MblMwDataProcessor *passthrough, uint16_t new_count) {
    if (passthrough->type == DataProcessorType::PASSTHROUGH) {
        set_processor_state(passthrough, &new_count, sizeof(new_count));
        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_passthrough_modify(MblMwDataProcessor *passthrough, MblMwPassthroughMode mode, uint16_t count) {
    if (passthrough->type == DataProcessorType::PASSTHROUGH) {
        PassthroughConfig* current_config= (PassthroughConfig*) passthrough->config;

        current_config->mode= mode;
        memcpy(((uint8_t*)(current_config)) + 1, &count, sizeof(count));

        modify_processor_configuration(passthrough, sizeof(PassthroughConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_pulse_create(MblMwDataSignal *source, MblMwPulseOutput output, float threshold,
        uint16_t width, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }
    
    int32_t scaled_threshold= (int32_t) number_to_firmware_converters.at(source->converter)(source, threshold);

    PulseDetectorConfig config;
    config.length= source->length() - 1;
    config.trigger_mode= 0;
    config.output_mode= output;
    memcpy(((uint8_t*)(&config)) + 3, &scaled_threshold, sizeof(scaled_threshold));
    memcpy(((uint8_t*)(&config)) + 7, &width, sizeof(width));
    
    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::PULSE), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_pulse_modify(MblMwDataProcessor *pulse, float threshold, uint16_t width) {
    if (pulse->type == DataProcessorType::PULSE) {
        int32_t scaled_threshold= (int32_t) number_to_firmware_converters.at(pulse->converter)(pulse, threshold);
        memcpy(((uint8_t*)(pulse->config)) + 3, &scaled_threshold, sizeof(scaled_threshold));
        memcpy(((uint8_t*)(pulse->config)) + 7, &width, sizeof(width));

        modify_processor_configuration(pulse, sizeof(PulseDetectorConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_sample_create(MblMwDataSignal *source, uint8_t bin_size, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    SampleDelayConfig config;
    config.length= source->length() - 1;
    config.bin_size= bin_size;
    
    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::SAMPLE), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_sample_modify_bin_size(MblMwDataProcessor *sample_delay, uint8_t bin_size) {
    if (sample_delay->type == DataProcessorType::SAMPLE) {
        SampleDelayConfig* current_config = (SampleDelayConfig*) sample_delay->config;
        current_config->bin_size= bin_size;
        modify_processor_configuration(sample_delay, sizeof(SampleDelayConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

int32_t mbl_mw_dataprocessor_threshold_create(MblMwDataSignal *source, MblMwThresholdMode mode, float boundary,
        float hysteresis, MblMwFnDataProcessor processor_created) {
    if (source->length() > PROCESSOR_MAX_LENGTH) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    int32_t scaled_boundary= (int32_t) number_to_firmware_converters.at(source->converter)(source, boundary);
    uint16_t scaled_hysteresis= (uint16_t) number_to_firmware_converters.at(source->converter)(source, hysteresis);

    ThresholdConfig config;
    *((uint8_t*) &config)= 0;
    config.length= source->length() - 1;
    config.is_signed= source->is_signed;
    config.mode= mode;

    memcpy(((uint8_t*)(&config)) + 1, &scaled_boundary, sizeof(scaled_boundary));
    memcpy(((uint8_t*)(&config)) + 5, &scaled_hysteresis, sizeof(scaled_hysteresis));

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::THRESHOLD), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_threshold_modify_boundary(MblMwDataProcessor *threshold, float boundary, float hysteresis) {
    if (threshold->type == DataProcessorType::THRESHOLD) {
        int32_t scaled_boundary= (int32_t) number_to_firmware_converters.at(threshold->converter)(threshold, boundary);
        uint16_t scaled_hysteresis= (uint16_t) number_to_firmware_converters.at(threshold->converter)(threshold, hysteresis);

        memcpy(((uint8_t*)(threshold->config)) + 1, &scaled_boundary, sizeof(scaled_boundary));
        memcpy(((uint8_t*)(threshold->config)) + 5, &scaled_hysteresis, sizeof(scaled_hysteresis));

        modify_processor_configuration(threshold, sizeof(ThresholdConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}

const uint8_t TIME_PASSTHROUGH_REVISION = 1;

int32_t mbl_mw_dataprocessor_time_create(MblMwDataSignal *source, MblMwTimeMode mode, uint32_t period,
        MblMwFnDataProcessor processor_created) {
    bool hasPassthrough = source->owner->module_info.at(MBL_MW_MODULE_DATA_PROCESSOR).revision >= TIME_PASSTHROUGH_REVISION;
    if (source->length() > PROCESSOR_MAX_LENGTH && (!hasPassthrough || mode == MBL_MW_TIME_DIFFERENTIAL)) {
        return MBL_MW_STATUS_ERROR_UNSUPPORTED_PROCESSOR;
    }

    TimeDelayConfig config;
    *((uint8_t*) &config)= 0;
    config.length= source->length() - 1;
    config.mode= (mode == MBL_MW_TIME_ABSOLUTE) && hasPassthrough ? 2 : mode;

    memcpy(((uint8_t*)(&config)) + 1, &period, sizeof(period));

    create_processor(source, MblMwDataProcessor::transform(source, type_to_id.at(DataProcessorType::TIME), &config), processor_created);

    return MBL_MW_STATUS_OK;
}

int32_t mbl_mw_dataprocessor_time_modify_period(MblMwDataProcessor *time_delay, uint32_t period) {
    if (time_delay->type == DataProcessorType::TIME) {
        memcpy(((uint8_t*)(time_delay->config)) + 1, &period, sizeof(period));
        modify_processor_configuration(time_delay, sizeof(TimeDelayConfig));

        return MBL_MW_STATUS_OK;
    }
    return MBL_MW_STATUS_WARNING_INVALID_PROCESSOR_TYPE;
}
