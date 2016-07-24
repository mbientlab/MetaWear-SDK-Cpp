#include "metawear/processor/passthrough.h"

#include "processor_private_common.h"

#include <cstdlib>
#include <cstring>

using std::malloc;
using std::memcpy;

struct PassthroughConfig {
    uint8_t mode;
    uint8_t count[2];
};

int32_t mbl_mw_dataprocessor_passthrough_create(MblMwDataSignal *source, MblMwPassthroughMode mode, uint16_t count,
        MblMwFnDataProcessor processor_created) {
    MblMwDataProcessor *new_processor = new MblMwDataProcessor(*source);
    create_processor_state_signal(new_processor, DataInterpreter::UINT32);

    PassthroughConfig *config = (PassthroughConfig*) malloc(sizeof(PassthroughConfig));
    config->mode= mode;
    memcpy(((uint8_t*)(config)) + 1, &count, sizeof(count));
    create_processor(source, config, sizeof(PassthroughConfig), DataProcessorType::PASSTHROUGH, new_processor, processor_created);

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
