.. highlight:: cpp

Proximity Detector
==================
MetaDetector boards come equipped with a `TSL2671 <http://ams.com/eng/Products/Light-Sensors/Proximity/TSL26711>`_ proximity detector.  Functions 
communicating with the detector are defined in `proximity_tsl2671.h <https://mbientlab.com/docs/metawear/cpp/latest/proximity__tsl2671_8h.html>`_ 
header file.

Configuration
-------------
The TSL2671 detector has 4 configurable parameters that control the sensitivity and distance at which the detector can measure proximity:

===================  ===================================================================
Parameter            Description
===================  ===================================================================
Integration Time     How long the internal ADC converts analog input into digital counts
Pulse Count          Number of IR pulses emitted for distance measuring
Receiver Diode       Which photodiode to use for measure incoming light
Transmitter Current  Amount of current driving the IR transmitter
===================  ===================================================================

It is recommended that you use the channel 1 receiver ,and set the transmitter current to 25mA or less if your board is powered by a CR2032 battery.  
To write your settings to the sensor, call 
`mbl_mw_proximity_tsl2671_write_config <https://mbientlab.com/docs/metawear/cpp/latest/proximity__tsl2671_8h.html#ac1df33796662b4a7e24e81cd9e89466f>`_ 
after choosing your desired settings. ::

    #include "metawear/sensor/proximity_tsl2671.h"
    
    void configure_proximity(MblMwMetaWearBoard* board) {
        // Set the integration time 5.44ms
        mbl_mw_proximity_tsl2671_set_integration_time(board, 5.44);
    
        // Use 32 pulses to sense proximity
        mbl_mw_proximity_tsl2671_set_n_pulses(board, 32);
    
        // Use receiver channel 1
        mbl_mw_proximity_tsl2671_set_receiver_channel(board, MBL_MW_PROXIMITY_TSL2671_CHANNEL_1);
    
        // Use a transmitter current of 12.5mA
        mbl_mw_proximity_tsl2671_set_transmitter_current(board, 
                MBL_MW_PROXIMITY_TSL2671_CURRENT_12_5MA);
    
        // Write configuration to the sensor
        mbl_mw_proximity_tsl2671_write_config(board);
    }

Distance Measurement
--------------------
Measuring distance is manually triggred by calling 
`mbl_mw_datasignal_read <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a0a456ad1b6d7e7abb157bdf2fc98f179>`_ with a proximity data 
signal. The distance data is expressed as an ADC value that is inversely proportional to distance; the ADC value is represented as an unsigned integer. ::

    #include "metawear/core/datasignal.h"
    #include "metawear/sensor/proximity_tsl2671.h"
    
    void proximity_stream(MblMwMetaWearBoard* board) {
        auto signal = mbl_mw_proximity_tsl2671_get_adc_data_signal(board);

        mbl_mw_datasignal_subscribe(signal, [](const MblMwData* data) -> void {
            // cast to uint32_t*
            printf("distance adc= %d\n", *((uint32_t*) data->value));
        });
        mbl_mw_datasignal_read(signal);
    }

