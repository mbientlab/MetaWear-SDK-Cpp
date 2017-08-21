.. highlight:: cpp

Barometer
=========
MetaWear RPro and Cpro, and MetaEnvironment boards come with a Bosch barometer.  The specific barometer model varies between the boards although both 
barometers are nearly identical save for a few settings.  Bosch barometer functions are defined in the 
`barometer_bosch.h <https://mbientlab.com/docs/metawear/cpp/latest/barometer__bosch_8h.html>`_ header file where functions containing ``baro_bosch`` 
are barometer agnostic where as functions with ``baro_bmp280`` and ``baro_bme280`` are for those specific barometers. 

Users can programatically determine which barometer is on their board with the 
`mbl_mw_metawearboard_lookup_module <https://mbientlab.com/docs/metawear/cpp/latest/metawearboard_8h.html#ad9c7e7f60f77fc1e929ac48c6a3ffb9b>`_ function. ::

    #include "metawear/sensor/barometer_bosch.h"
    
    void barometer_who_am_i(MblMwMetaWearBoard* board) {
        switch (mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_BAROMETER)) {
        case MBL_MW_MODULE_TYPE_NA:
            printf("no barometer on this board\n");
            break;
        case MBL_MW_MODULE_BARO_TYPE_BMP280:
            printf("barometer type = BMP280\n");
            break;
        case MBL_MW_MODULE_BARO_TYPE_BME280:
            printf("barometer type = BME280\n");
            break;
        default:
            printf("unknown barometer type\n");
            break;
        }
    }

Sensor Configuration
--------------------
The Bosch barometers have 3 configurable parameters: 

* Oversampling
* Infinite impulse filter (iir) coefficient
* Standby time

These operational parameters work in conjunction to control the noise, output resolution, and sampling rate.  When you are done setting the configuration, 
call `mbl_mw_baro_bosch_write_config <https://mbientlab.com/docs/metawear/cpp/latest/barometer__bosch_8h.html#ac763f27505e504d7c7ebd37c7bc98aa6>`_ to 
write the changes to the sensor. ::

    void configure_barometer(MblMwMetaWearBoard* board) {
        // Set oversampling to ultra high resolution
        mbl_mw_baro_bosch_set_oversampling(board, MBL_MW_BARO_BOSCH_OVERSAMPLE_ULTRA_HIGH);
    
        // Set standby time to 62.5ms or closest valid valud
        mbl_mw_baro_bosch_set_standby_time(board, 62.5f);
    
        // Set iir filter coefficient
        mbl_mw_baro_bosch_set_iir_filter(board, MBL_MW_BARO_BOSCH_IIR_FILTER_AVG_4);
    
        // Write configuration to the sensor
        mbl_mw_baro_bosch_write_config(board);
    }

Pressure Sampling
-----------------
Pressure data is represented as a float and is in units of Pascals.   To receive pressure data, simply subscribe or log the pressure data signal and 
then start the sensor. ::

    #include "metawear/core/datasignal.h"
    
    void setup_pressure_stream(MblMwMetaWearBoard* board) {
        auto data_handler = [](const MblMwData* data) -> void {
            // Cast value to float*
            printf("%.3fPa\n", *((float*) data->value));
        };
        auto pa_signal= mbl_mw_baro_bosch_get_pressure_data_signal(board);
    
        mbl_mw_datasignal_subscribe(pa_signal, data_handler);
        mbl_mw_baro_bosch_start(board);
    }

Altitude Sampling
-----------------
Altitude data is represented as a float and is in units of meters.  To receive altitude data, simply subscribe or log the altitude data signal and then 
start the sensor. :: 

    #include "metawear/core/datasignal.h"
    
    void setup_altitude_stream(MblMwMetaWearBoard* board) {
        auto data_handler = [](const MblMwData* data) -> void {
            // Cast value to float*
            printf("%.3fm\n", *((float*) data->value));
        };
        auto m_signal= mbl_mw_baro_bosch_get_altitude_data_signal(board);
    
        mbl_mw_datasignal_subscribe(m_signal, data_handler);
        mbl_mw_baro_bosch_start(board);
    }
