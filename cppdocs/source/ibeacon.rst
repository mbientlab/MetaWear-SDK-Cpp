.. highlight:: cpp

iBeacon
=======
The MetaWear firmware supports the iBeacon format and can advertise itself as an iBeacon.  To enable iBeacon mode, all you need to do is call 
`mbl_mw_ibeacon_enable <https://mbientlab.com/docs/metawear/cpp/latest/ibeacon_8h.html#a29227024839d419f2d536b6b3cc42481>`_ and disconnect from the 
board.  The other functions in the `ibeacon.h <https://mbientlab.com/docs/metawear/cpp/latest/ibeacon_8h.html>`_ header file configure the 
advertisement parameters. ::

    #include "metawear/peripheral/ibeacon.h"
    
    void ibeacon_enable(MblMwMetaWearBoard* board) {
        // Set major number to 31415
        mbl_mw_ibeacon_set_major(board, 31415);
    
        // Set minor number to 9265
        mbl_mw_ibeacon_set_minor(board, 9265);
    
        // Enable ibeacon mode
        mbl_mw_ibeacon_enable(board);
    }

