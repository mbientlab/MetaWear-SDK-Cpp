#include "metawear/switch.h"

int mbl_mw_switch_get_data(uint8_t *data, uint8_t response[3]) {
    if (response[0] == MBL_MW_SWITCH_MODULE && response[1] == MBL_MW_SWITCH_DATA) {
        *data= response[2];
        return STATUS_OK;
    }
    return STATUS_INVALID_RESPONSE;
}
