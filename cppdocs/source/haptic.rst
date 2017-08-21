.. highlight:: cpp

Haptic
======
The haptic module controls a high current driver to power a motor or buzzr; functions are defined in the 
`haptic.h <https://mbientlab.com/docs/metawear/cpp/latest/haptic_8h.html>`_ header filer.  Circuit diagrams for the driver pin are in section 8 of the 
`product specification <https://mbientlab.com/docs/MetaWearPPSv0.7.pdf>`_. ::

    #include "metawear/peripheral/haptic.h"
    
    void run_haptic(MblMwMetaWearBoard* board) {
        // Run buzzer for 500ms
        mbl_mw_haptic_start_buzzer(board, 500);
    
        // Run motor at 50% strength for 1000ms
        mbl_mw_haptic_start_motor(board, 50.f, 1000);
    }

