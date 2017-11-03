.. highlight:: cpp

Data Processor Types
====================
Header files defining the data processors type are in the 
`processor <https://mbientlab.com/docs/metawear/cpp/latest/dir_ac375e5396e5f8152317e89ec5f046d1.html>`_ folder.  To create a processor, call any 
functions that has ``create`` in its name.  All data processor create functions are asynchronous and alert the caller when the processor is created on 
board through callback functions.

Input data signals that are marked with a `MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ id, 
.i.e accelerometer, gyro, and magnetometer data, are limited to only using the :ref:`dataprocessor-math`, :ref:`dataprocessor-rms`, and 
:ref:`dataprocessor-rss` processors.  Once fed through an RMS or RSS processor however, they can utilize the rest of the data processing functions.

Accounter
---------
The accounter processor adds additional information to the BTLE packet to reconstruct the data's timestamp, typically used with streaming raw 
accelerometer, gyro, and magnetometer data.  This processor is designed specifically for streaming, do not use with the logger.  ::

    #include "metawear/processor/accounter.h"

    void account_acc_data(MblMwMetaWearBoard* board) {
        auto signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_dataprocessor_accounter_create(signal, [](MblMwDataProcessor* accounter) {
            mbl_mw_datasignal_subscribe((MblMwDataSignal*)accounter, [](const MblMwData* data) {
                cout << "real time = " << data->epoch << endl;
            });
        });
    }

Accumulator
-----------
The accumulator computes a running sum over the inputs.  Users can explicitly specify an output size (1 to 4 bytes) or 
let the API infer an appropriate size.  The output data type id of an accumulator is the same as its input source. ::

    #include "metawear/processor/accumulator.h"

    void create_accumulator(MblMwDataSignal* signal) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("accumulator created\n");
        };

        // API will determine how many bytes to allocate for the sum
        mbl_mw_dataprocessor_accumulator_create(signal, proc_created);
    }

Average
-------
The averager computes a running average over the over the inputs.  It will not produce any output until it has accumulated enough samples to match 
the specified sample size. The output data type id of averager is the same as its input source. ::

    #include "metawear/processor/average.h"
    #include "metawear/sensor/gpio.h"

    void create_averager(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("averager created\n");
        };

    	auto abs_ref_signal = mbl_mw_gpio_get_analog_input_data_signal(board, 0,
            MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF);
        // Compute running average using the previous 4 samples
        mbl_mw_dataprocessor_average_create(abs_ref_signal, 4, proc_created);
    }

Buffer
------
The buffer processor captures input data which can be read at a later time using 
`mbl_mw_datasignal_read <https://mbientlab.com/docs/metawear/cpp/latest/datasignal_8h.html#a0a456ad1b6d7e7abb157bdf2fc98f179>`_; no output is produced 
by this processor.  The data type id of a buffer's state is the same as its input source. ::

    #include "metawear/processor/buffer.h"

    void create_buffer(MblMwDataSignal* signal) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("buffer created\n");
        };

        mbl_mw_dataprocessor_buffer_create(signal, proc_created);
    }

Buffer processors can be used to capture data and retrieve it at a later time by reading its state.

Comparison
----------
The comparator removes data that does not satisfy the comparison operation.  Callers can force a signed or unsigned comparison, or let the API 
determine which is appropriate.  The output data type id of comparator is the same as its input source. ::

    #include "metawear/processor/comparator.h"
    #include "metawear/sensor/multichanneltemperature.h"

    void setup_temp_comparator(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("comparison processor created\n");
        };

        auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board,
            MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR);

        // only allow temperature data greater than 25C through
        mbl_mw_dataprocessor_comparator_create(temp_signal, MBL_MW_COMPARATOR_OP_GT, 25.f, 
                proc_created);
    }

Comparators can also be used in feedback/feedforward loops.  Using 
`mbl_mw_dataprocessor_comparator_modify_signal <https://mbientlab.com/docs/metawear/cpp/latest/comparator_8h.html#a30b619b26825669973195bf19eee6b3d>`_, 
you can compare input data against a dynamic reference value. ::

    #include "metawear/core/event.h"
    #include "metawear/processor/comparator.h"
    #include "metawear/sensor/multichanneltemperature.h"

    void setup_temp_comparator(MblMwMetaWearBoard* board) {
        static auto cmds_recorded = [](void) -> void {
            printf("feedback loop created\n");
        };
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("comparison processor created\n");

            mbl_mw_event_record_commands((MblMwEvent*) processor);
            // Whenever data is received that satisfies the comparison,  update the reference value 
            mbl_mw_dataprocessor_comparator_modify_signal(processor, MBL_MW_COMPARATOR_OP_GT, 
                    (MblMwDataSignal*) processor);
            mbl_mw_event_end_record((MblMwEvent*)processor, cmds_recorded);
        };

        auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board,
            MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR);

        // only allow temperature data greater than 25C through
        mbl_mw_dataprocessor_comparator_create(temp_signal, MBL_MW_COMPARATOR_OP_GT, 25.f, 
                proc_created);
    }

Multi-Value Comparison
^^^^^^^^^^^^^^^^^^^^^^
Starting from firmware v1.2.3, the comparator can accept multiple reference values to compare against and has additional operation modes that can 
modify output values and when outputs are produced.  The multi-value comparison filter is an extension of the comparison filter implemented on 
older firmware.

Operation modes are defined in the 
`MblMwComparatorOperation <https://mbientlab.com/docs/metawear/cpp/latest/comparator_8h.html#a021a5e13dd18fb4b5b2052bf547e5f54>`_ enum, copied below 
with a description on expected outputs:

===========  =====================================================================================================
Operation    Descripion
===========  =====================================================================================================
Absolute     Input value is returned when the comparison is satisfied, behavior of old comparator
Reference    The reference value is output when the comparison is satisfied
Zone         Outputs the index (0 based) of the reference value that satisfied the comparison, n if none are valid
Pass / Fail  0 if the comparison fails, 1 if it passed
===========  =====================================================================================================

Also note that you can only use one reference value when creating feedback/feedforward loops.  ::

    auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board,
            MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR);

    float references[4]= {18.f, 20.f, 22.f, 24.f};
    // Check which threshold was crossed
    mbl_mw_dataprocessor_multi_comparator_modify(temp_signal, MBL_MW_COMPARATOR_MODE_ZONE, 
            references, (uint8_t) sizeof(references), [](MblMwDataProcessor* processor) -> void {
        printf("comparison processor created\n");
    });

Counter
-------
A counter keeps a tally of how many times it is called.  It can be used by 
`MblMwEvent <https://mbientlab.com/docs/metawear/cpp/latest/event__fwd_8h.html#a569b89edd88766619bb41a2471743695>`_ pointers to count the numbers of 
times a MetaWear event was fired and enable simple events to utilize the full set of firmware features.  Counter data is only interpreted as an 
unsigned integer. ::

    #include "metawear/core/settings.h"
    #include "metawear/processor/counter.h"

    void setup_dc_counter(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("counter processor created\n");
        };

        auto dc_event = mbl_mw_settings_get_disconnect_event(board);
        // Count the number of times the disconnected event was fired
        mbl_mw_dataprocessor_counter_create(dc_event, proc_created);
    }

Delta
-----
A delta processor computes the difference between two successive data values and only allows data through that creates a difference greater in magnitude 
than the specified threshold.  When creating a delta processor, users will also choose how the processor transforms the output which can, in some cases, 
alter the output data type id.  

=============  =======================================  ==============================================
Output         Transformation                           Data Type ID
=============  =======================================  ==============================================
Absolute       Input passed through untouched           Same as input source i.e. float -> float
Differential   Difference between current and previous  If input is unsigned int, output is signed int
Binary         1 if difference > 0, -1 if less than 0   Output is always signed int
=============  =======================================  ==============================================

Constants identifying the output modes are defined in the `MblMwDeltaMode <https://mbientlab.com/docs/metawear/cpp/latest/delta_8h.html#ac9e3bece74c3bafb355bb158cf93b843>`_ enum. ::

    #include "metawear/processor/delta.h"
    #include "metawear/sensor/gpio.h"

    void setup_adc_delta(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("delta processor created\n");
        };

        auto adc_signal = mbl_mw_gpio_get_analog_input_data_signal(board, 0, 
                MBL_MW_GPIO_ANALOG_READ_MODE_ADC);
        // Use binary mode to transform the output to +1/-1
        // Only allow adc data through that creates an absolute difference of 128 or more
        mbl_mw_dataprocessor_delta_create(adc_signal, MBL_MW_DELTA_MODE_BINARY, 128, proc_created);
    }

High Pass Filter
----------------
High pass filters compute the difference of the current value from a running average of the previous N samples.  Output from this processor is delayed 
until the first N samples have been received.  ::

    #include "metawear/processor/average.h"

    void hpf_acc_data(MblMwMetaWearBoard* board) {
        auto signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_dataprocessor_highpass_create(signal, 4, [](MblMwDataProcessor* hpf) {
            mbl_mw_datasignal_subscribe((MblMwDataSignal*)hpf, [](const MblMwData* data) {
                auto value = (MblMwCartesianFloat*)data->value;
                printf("hpf acc = (%.3f, %.3f, %.3f)\n", value->x, value->y, value->z);
            });
        });
    }

.. _dataprocessor-math:

Math
----
The math processor performs arithmetic or logical operations on the input.  Users can force signed or unsigned operation, or allow the API to determine 
which is appropriate.  Depending on the operation, the output data type id can change.

========================    ====================================================
Operation                   Data Type ID
========================    ====================================================
Add, Sub, Mult, Div, Mod    If input is unsigned, output is signed
Sqrt, Abs                   If input is signed, output is unsigned
Const                       Output type id is the same as input type id
Remaining Ops               API cannot infer, up to user to reassemble the bytes
========================    ====================================================

Constants identifying the operations are defined in the 
`MblMwMathOperation <https://mbientlab.com/docs/metawear/cpp/latest/math_8h.html#acb93d624e6a4bdfcda9bac362197b232>`_ enum. ::

    #include "metawear/processor/math.h"
    #include "metawear/sensor/multichanneltemperature.h"

    void setup_adc_delta(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("math processor created\n");
        };

        auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board, 
                MBL_MW_METAWEAR_RPRO_CHANNEL_ON_DIE);
        // Added 273.15C to the input converting units to Kelvin
        mbl_mw_dataprocessor_math_create(temp_signal, MBL_MW_MATH_OP_ADD, 273.15, proc_created);
    }

Like the comparator, the math processor also supports feedback/feedforward loops.  Using 
`mbl_mw_dataprocessor_math_modify_rhs_signal <https://mbientlab.com/docs/metawear/cpp/latest/math_8h.html#a7c7af2e8139e824b82c45b846b96abc6>`_, you can 
set the second operand with the output of another data signal. ::

    #include "metawear/core/event.h"
    #include "metawear/processor/math.h"
    #include "metawear/sensor/switch.h"

    void switch_feedback(MblMwDataProcessor* math_processor) {
        static auto cmds_recorded = [](void) -> void {
            printf("feedback loop completed\n");
        };

        auto owner = mbl_mw_event_get_owner((MblMwEvent*)math_processor);
        auto switch_signal = mbl_mw_switch_get_state_data_signal(owner);

        // everytime the switch state changes, the second operand of the math operation will also 
        // change to match the switch state (1 or 0)
        mbl_mw_event_record_commands((MblMwEvent*) switch_signal);
        mbl_mw_dataprocessor_math_modify_rhs_signal(math_processor, switch_signal);
        mbl_mw_event_end_record((MblMwEvent*) switch_signal, cmds_recorded);
    }

Packer
------
The packer processor combines multiple data samples into 1 BLE packet to increase the data throughput.  You can pack between 4 to 8 samples per packet 
depending on the data size.

Note that if you use the packer processor with raw motion data instead of using their packed data producer variants, you will only be able to combine 2 
data samples into a packet instead of 3 samples however, you can chain an accounter processor to associate a timestamp with the packed data.  ::

    #include "metawear/processor/packer.h"

    int samples;
    void pack_data(MblMwDataSignal* signal) {
        mbl_mw_dataprocessor_packer_create(signal, 4, [](MblMwDataProcessor* packer) {
            samples = 0;
            mbl_mw_datasignal_subscribe((MblMwDataSignal*)packer, [](const MblMwData* data) {
                samples++;
                cout << "samples = " << samples << endl;
            });
        });
    }

Passthrough
-----------
The passthrough processor is akin to a gate in which the user has manual control over, exercised by setting the processor's count value using  
`mbl_mw_dataprocessor_passthrough_set_count <https://mbientlab.com/docs/metawear/cpp/latest/passthrough_8h.html#a537a105294960629fd035adac1a5d65b>`_.  
It has three operation modes that each use the count value differently:

=========== ==========================================
Mode        Description
=========== ==========================================
All         Allow all data through
Conditional Only allow data through if the count > 0
Count       Only allow a set number of samples through
=========== ==========================================

Constants identifying the operation modes are defined in the 
`MblMwPassthroughMode <https://mbientlab.com/docs/metawear/cpp/latest/passthrough_8h.html#a3fdd23d48b54420240c112fa811a56dd>`_ enum. ::

    #include "metawear/processor/passthrough.h"
    #include "metawear/sensor/gpio.h"

    void passthrough_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("passthrough processor created\n");
        };

        auto abs_gpio_signal = mbl_mw_gpio_get_analog_input_data_signal(board, 0, 
                MBL_MW_GPIO_ANALOG_READ_MODE_ABS_REF);
        // Create a passthrough processor in count mode
        // only allows 16 data samples through, then block all other samples
        mbl_mw_dataprocessor_passthrough_create(abs_gpio_signal, MBL_MW_PASSTHROUGH_COUNT, 16, 
                proc_created);
    }

Pulse
-----
The pulse processor detects and quantifies a pulse over a set of data.  Pulses are defined as a minimum number of data points that rise above then fall 
below a threshold and quantified by transforming the collection of data into three different values:

========= ======================================== =================================
Output    Description                              Data Type ID
========= ======================================== =================================
Width     Number of samples that made up the pulse Unsigned integer
Area      Summation of all the data in the pulse   Same as input i.e. float -> float
Peak      Highest value in the pulse               Same as input i.e. float -> float
On Detect Return 0x1 as soon as pulse is detected  Unsigned integer
========= ======================================== =================================

Constants defining the different output modes are defined in the 
`MblMwPulseOutput <https://mbientlab.com/docs/metawear/cpp/latest/pulse_8h.html#abd7edcb82fd29ec984390673c60b4904>`_ enum. ::

    #include "metawear/processor/pulse.h"
    #include "metawear/sensor/gpio.h"

    void passthrough_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("pulse processor created\n");
        };

        auto adc_gpio_signal = mbl_mw_gpio_get_analog_input_data_signal(board, 0, 
                MBL_MW_GPIO_ANALOG_READ_MODE_ADC);
        // values must rise above then fall below 512 and have a min of 16 values
        // the highest value in the collected data will be returned
        mbl_mw_dataprocessor_pulse_create(adc_gpio_signal, MBL_MW_PULSE_OUTPUT_PEAK, 512.f, 16, 
                proc_created);
    }

.. _dataprocessor-rms:

RMS
---
The RMS processor computes the root mean square over multi component data i.e. XYZ values from acceleration data.  The processor will convert 
`MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ inputs into float outputs.  ::

    #include "metawear/processor/rms.h"
    #include "metawear/sensor/accelerometer.h"

    void create_rms_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("rms processor created\n");
        };

        auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_dataprocessor_rms_create(acc_signal, proc_created);
    }

.. _dataprocessor-rss:

RSS
---
The RSS processor computes the root sum square, or vector magnitude, over multi component data i.e. XYZ values from acceleration data.  The processor 
will convert `MblMwCartesianFloat <https://mbientlab.com/docs/metawear/cpp/latest/structMblMwCartesianFloat.html>`_ inputs into float outputs.  ::

    #include "metawear/processor/rms.h"
    #include "metawear/sensor/accelerometer.h"

    void create_rss_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("rss processor created\n");
        };

        auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_dataprocessor_rss_create(acc_signal, proc_created);
    }

Sample
------
The sample processor acts like a bucket, only allowing data through once it has collected a set number of samples. It functions as a data historian of 
sorts providing a way to look at the data values prior to an event.  The output data type id of an accumulator is the same as its input source. ::

    #include "metawear/processor/sample.h"
    #include "metawear/sensor/switch.h"

    void sample_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("sample processor created\n");
        };

        auto switch_signal = mbl_mw_switch_get_state_data_signal(board);
        // collect 16 samples of switch state data before allowing data to pass
        mbl_mw_dataprocessor_sample_create(switch_signal, 16, proc_created);
    }

Threshold
---------
The threshold processor only allows data through that crosses a boundary, either crossing above or below it.  It has two output modes:

=============  ========================================== ==============================================
Output         Transformation                             Data Type ID
=============  ========================================== ==============================================
Absolute       Input passed through untouched             Same as input source i.e. float -> float
Binary         1 if value rose above, -1 if it fell below Output is always signed int
=============  ========================================== ==============================================

Constants identifying the output modes are defined by the 
`MblMwThresholdMode <https://mbientlab.com/docs/metawear/cpp/latest/threshold_8h.html#a63e1cc001aa56601099db511d3d3109c>`_ enum. ::

    #include "metawear/processor/threshold.h"
    #include "metawear/sensor/multichanneltemperature.h"

    void threshold_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("threshold processor created\n");
        };

        auto temp_signal = mbl_mw_multi_chnl_temp_get_temperature_data_signal(board, 
                MBL_MW_METAWEAR_RPRO_CHANNEL_ON_BOARD_THERMISTOR);
        // only allow data through when it rises above or falls below 25C
        mbl_mw_dataprocessor_threshold_create(temp_signal, MBL_MW_THRESHOLD_MODE_BINARY, 25, 0, 
                proc_created);
    }

Time
----
The time processor only allows data to pass at fixed intervals.  It can used to limit the rate at which data is received if your sensor does not have 
the desired sampling rate.  The processor has two output modes:

=============  ======================================= ==============================================
Output         Transformation                          Data Type ID
=============  ======================================= ==============================================
Absolute       Input passed through untouched          Same as input source i.e. float -> float
Differential   Difference between current and previous If input is unsigned int, output is signed int
=============  ======================================= ==============================================

Constants identifying the the output modes are defined by the 
`MblMwTimeMode <https://mbientlab.com/docs/metawear/cpp/latest/time_8h.html#ac5166dcd417797f9bc13a5e388d9073c>`_. ::

    #include "metawear/processor/time.h"
    #include "metawear/sensor/accelerometer.h"

    void threshold_processor(MblMwMetaWearBoard* board) {
        static auto proc_created = [](MblMwDataProcessor* processor) -> void {
            printf("time processor created\n");
        };

        auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
        // reduce accelerometer data rate to 125ms or 8Hz
        mbl_mw_dataprocessor_time_create(acc_signal, MBL_MW_TIME_ABSOLUTE, 125, proc_created);
    }
