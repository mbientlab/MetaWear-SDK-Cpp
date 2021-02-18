#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#define METAWEAR_HELPER_DLL_IMPORT __declspec(dllimport)
#define METAWEAR_HELPER_DLL_EXPORT __declspec(dllexport)
#define METAWEAR_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define METAWEAR_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define METAWEAR_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define METAWEAR_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define METAWEAR_HELPER_DLL_IMPORT
#define METAWEAR_HELPER_DLL_EXPORT
#define METAWEAR_HELPER_DLL_LOCAL
#endif
#endif

#if defined _WINDLL || defined METAWEAR_DLL // defined if METAWEAR is compiled as a DLL
#ifdef METAWEAR_DLL_EXPORTS
/** Indicates the function should be exported to the symbol table  */
#define METAWEAR_API METAWEAR_HELPER_DLL_EXPORT
#else
#define METAWEAR_API METAWEAR_HELPER_DLL_IMPORT
#endif // METAWEAR_DLL_EXPORTS
/** Indicates the function is only to be used by the API */
#define METAWEAR_LOCAL METAWEAR_HELPER_DLL_LOCAL
#else // METAWEAR_DLL is not defined: this means METAWEAR is a static lib.
#define METAWEAR_API
#define METAWEAR_LOCAL
#endif // METAWEAR_DLL

/**
 * Sensors or peripherals supported by the firmware.  Different board skews have different module combinations, 
 * use mbl_mw_metawearboard_lookup_module to check if a module is present on your board
 */
typedef enum {
    MBL_MW_MODULE_SWITCH = 1,
    MBL_MW_MODULE_LED,
    MBL_MW_MODULE_ACCELEROMETER,
    MBL_MW_MODULE_TEMPERATURE,
    MBL_MW_MODULE_GPIO,
    MBL_MW_MODULE_NEO_PIXEL,
    MBL_MW_MODULE_IBEACON,
    MBL_MW_MODULE_HAPTIC,
    MBL_MW_MODULE_DATA_PROCESSOR,
    MBL_MW_MODULE_EVENT,
    MBL_MW_MODULE_LOGGING,
    MBL_MW_MODULE_TIMER,
    MBL_MW_MODULE_I2C,
    MBL_MW_MODULE_MACRO = 0xf,
    MBL_MW_MODULE_CONDUCTANCE,
    MBL_MW_MODULE_SETTINGS,
    MBL_MW_MODULE_BAROMETER,
    MBL_MW_MODULE_GYRO,
    MBL_MW_MODULE_AMBIENT_LIGHT,
    MBL_MW_MODULE_MAGNETOMETER,
    MBL_MW_MODULE_HUMIDITY,
    MBL_MW_MODULE_COLOR_DETECTOR,
    MBL_MW_MODULE_PROXIMITY,
    MBL_MW_MODULE_SENSOR_FUSION,
    MBL_MW_MODULE_DEBUG = 0xfe
} MblMwModule;

typedef enum {
    MBL_MW_GATT_CHAR_WRITE_WITH_RESPONSE = 0,
    MBL_MW_GATT_CHAR_WRITE_WITHOUT_RESPONSE
} MblMwGattCharWriteType;

typedef struct {
    uint64_t service_uuid_high;         ///< High 64 bits of the parent service uuid
    uint64_t service_uuid_low;          ///< Low 64 bits of the parent service uuid
    uint64_t uuid_high;                 ///< High 64 bits of the characteristic uuid
    uint64_t uuid_low;                  ///< Low 64 bits of the characteristic uuid
} MblMwGattChar;

typedef struct {
    /** 
     * Writes the characteristic and value to the device
     * @param caller            Object using this function pointer
     * @param characteristic    Gatt characteristic to write
     * @param value             Value to write as a byte array
     * @param length            Length of the byte array
     */
    void (*write_gatt_char)(const void* caller, MblMwGattCharWriteType writeType, const MblMwGattChar* characteristic, 
            const uint8_t* value, uint8_t length);
    void (*read_gatt_char)(const void* caller, const MblMwGattChar* characteristic);
} MblMwBtleConnection;

typedef struct {
    float w;
    const float *x;
    float y;
    float z;
} MblMwQuaternion;

struct MblMwMetaWearBoard;

typedef void(*MblMwFnBoardPtrInt)(MblMwMetaWearBoard* board, int32_t status);
/**
 * Serializes the API state.  The memory allocated by the function must be freed by calling mbl_mw_memory_free.
 * @param board         Board to serialize
 * @param size          Pointer to where the size of the returned byte array will be written to
 * @return Byte array of the serialized state
 */
METAWEAR_API uint8_t* mbl_mw_metawearboard_serialize(const MblMwMetaWearBoard* board, uint32_t* size);

METAWEAR_API void mbl_mw_metawearboard_initialize(MblMwMetaWearBoard *board, MblMwFnBoardPtrInt initialized);

METAWEAR_API void mbl_mw_ibeacon_set_uuid(const MblMwMetaWearBoard *board, uint8_t ad_uuid[16]);

/** Indices for component values of the acceleration data signal, used with mbl_mw_datasignal_get_component */
const uint8_t MBL_MW_ACC_ACCEL_X_AXIS_INDEX = 0, 
        MBL_MW_ACC_ACCEL_Y_AXIS_INDEX = 1, 
        MBL_MW_ACC_ACCEL_Z_AXIS_INDEX = 2;
const MblMwGattChar METAWEAR_SERVICE_NOTIFY_CHAR = { 0x326a900085cb9195, 0xd9dd464cfbbae75a, 0x326a900685cb9195, 0xd9dd464cfbbae75a };

struct MblMwAnonymousDataSignal;

typedef void(*MblMwFnAnonSignalArray)(MblMwMetaWearBoard* board, MblMwAnonymousDataSignal** anonymous_signals, uint32_t size);

METAWEAR_API void mbl_mw_metawearboard_create_anonymous_datasignals(MblMwMetaWearBoard* board, MblMwFnAnonSignalArray created);
