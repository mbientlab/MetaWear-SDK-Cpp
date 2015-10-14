/**
* @copyright MbientLab License
* @file metawearboard.h
*/
#pragma once

#include <stdint.h>

#include "dllmarker.h"
#include "metawearboard_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates an instance of the MblMwMetaWearBoard struct
 * @return Pointer to the newly created struct
 */
METAWEAR_API MblMwMetaWearBoard* mbl_mw_create_metawear_board();
/**
 * Frees the memory allocated for the struct
 * @param board     Pointer to the memory to free
 */
METAWEAR_API void mbl_mw_free_metawear_board(MblMwMetaWearBoard *board);
//METAWEAR_API void mbl_mw_metawearboard_discover_modules();

/**
 * Handles responses from the MetaWearBoard and must be called for every response received
 * @param board         Pointer to the board the response is from
 * @param response      Byte array holding the response
 * @param len           Length of the array
 */
METAWEAR_API int32_t mbl_mw_metawearboard_handle_response(const MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len);

#ifdef __cplusplus
}
#endif
