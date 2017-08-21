.. highlight:: cpp

Ambient Light Sensor
====================
MetaWear RPro and Cpro, and MetaDetector board come with a 
`Lite-On LTR-329ALS <http://www.mouser.com/ds/2/239/Lite-On_LTR-329ALS-01%20DS_ver1.1-348647.pdf>`_ ambient light sensor.  Functions interacting with 
the light sensor are defined in the `ambientlight_ltr329.h <https://mbientlab.com/docs/metawear/cpp/latest/ambientlight__ltr329_8h.html>`_ header file.

Configuration
-------------
The LTR329 sensor has 3 configurable parameters:

================  =========================================
Parameter         Description
================  =========================================
Gain              Controls data range and resolution
Integration Time  Measurement time for each cycle
Measurement Rate  How frequently to update illuminance data
================  =========================================

Possible values for each of these parameters are defined in their respective enums.  After configuring the API with the desired settings, call 
`mbl_mw_als_ltr329_write_config <https://mbientlab.com/docs/metawear/cpp/latest/ambientlight__ltr329_8h.html#ad37c6a477bde0813186694bff2bcd972>`_. to 
write the settings to the sensor. ::

    #include "metawear/sensor/ambientlight_ltr329.h"
    
    void configure_als(MblMwMetaWearBoard* board) {
        // Set sensor gain to 96x
        mbl_mw_als_ltr329_set_gain(board, MBL_MW_ALS_LTR329_GAIN_96X);
    
        // Set the integration time to 400ms
        mbl_mw_als_ltr329_set_integration_time(board, MBL_MW_ALS_LTR329_TIME_400MS);
    
        // Set the measurement rate to 1000ms
        mbl_mw_als_ltr329_set_measurement_rate(board, MBL_MW_ALS_LTR329_RATE_1000MS);
    
        // Write the configuration to the sensor
        mbl_mw_als_ltr329_write_config(board);
    }

Illuminance Measurement
-----------------------
To start measuring illuminance, call  
`mbl_mw_als_ltr329_start <https://mbientlab.com/docs/metawear/cpp/latest/ambientlight__ltr329_8h.html#a6e490d629752ddd32bb2a2b39c3ca1fc>`_.  
Illuminance data is represented as an unsigned integer and is in units of milli lux. ::

    #include "metawear/core/datasignal.h"
    
    void setup_illuminance_stream(MblMwMetaWearBoard* board) {
        auto data_handler = [](const MblMwData* data) -> void {
            // Cast value to uint32_t*
            printf("%dmlx\n", *((uint32_t*) data->value));
        };
        auto acc_signal= mbl_mw_als_ltr329_get_illuminance_data_signal(board);
    
        mbl_mw_datasignal_subscribe(acc_signal, data_handler);
        mbl_mw_als_ltr329_start(board);
    }

