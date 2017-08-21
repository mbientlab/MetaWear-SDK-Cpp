.. highlight:: cpp

Color Detector
==============
MetaEnvironment boards come equipped with a `TCS34725 <http://ams.com/eng/Products/Light-Sensors/Color-Sensors/TCS34725>`_ color detector.  Enums and 
functions for interacting with this sensor are defined in the 
`colordetector_tcs34725.h <https://mbientlab.com/docs/metawear/cpp/latest/colordetector__tcs34725_8h.html>`_ header file.

Configuration
-------------
The `TCS34725 <http://ams.com/eng/Products/Light-Sensors/Color-Sensors/TCS34725>`_ sensor has 2 configurable parameters that affect the data range and 
resultion: 

================  ============================================
Parameter         Description
================  ============================================
Gain              Analog signal scale
Integration Time  Amount of time spent to aggregate adc values
================  ============================================

Afer you configure the API with your desired settings, call 
`mbl_mw_cd_tcs34725_write_config <https://mbientlab.com/docs/metawear/cpp/latest/colordetector__tcs34725_8h.html#a78084a7cc57f0a56eb5b338aca732138>`_ 
to write the changes to the detector. ::

    #include "metawear/sensor/colordetector_tcs34725.h"
    
    void colordetector_config(MblMwMetaWearBoard* board) {
        // set analog gain to 16x
        mbl_mw_cd_tcs34725_set_gain(board, MBL_MW_CD_TCS34725_GAIN_16X);
    
        // set integration time to 4.8ms
        mbl_mw_cd_tcs34725_set_integration_time(board, 4.8f);
    
        // write the configuration to the sensor
        mbl_mw_cd_tcs34725_write_config(board);
    }

The MetaWear board also comes with an illuminator LED which can be used to provide additional light when reading the color.  To enable the LED, call 
`mbl_mw_cd_tcs34725_enable_illuminator_led <https://mbientlab.com/docs/metawear/cpp/latest/colordetector__tcs34725_8h.html#abdd816e83bbc3a550b5ba6a50d803729>`_ before writing the configuration changes.

Color ADC
---------
To retrieve color adc, call 
`mbl_mw_datasignal_read <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a0a456ad1b6d7e7abb157bdf2fc98f179>`_ with a color adc data 
signal.  Users can use the timer functions (see :doc:`timer` section) 
to schedule periodic reads.  ::

    #include "metawear/core/datasignal.h"
    #include "metawear/core/types.h"
    
    void colordetector_adc_stream(MblMwMetaWearBoard* board) {
        auto adc_signal= mbl_mw_cd_tcs34725_get_adc_data_signal(board);
    
        mbl_mw_datasignal_subscribe(adc_signal, [](const MblMwData* data) -> void {
            // Cast value to MblMwTcs34725ColorAdc*
            auto color_adc = (MblMwTcs34725ColorAdc*) data->value;
            printf("{clear: %d, red: %d, green: %d, blue: %d}", color_adc->clear, 
                    color_adc->red, color_adc->green, color_adc->blue);
        });
        mbl_mw_datasignal_read(adc_signal);
    }

