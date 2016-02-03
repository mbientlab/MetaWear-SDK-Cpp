#include "metawear/core/event.h"
#include "event_register.h"
#include "event_private.h"

#include "metawearboard_def.h"

MblMwEvent::MblMwEvent(const ResponseHeader& header, MblMwMetaWearBoard *owner) : header(header), owner(owner), remove(true) {
}

MblMwEvent::~MblMwEvent() {
    if (remove) {
        uint8_t command[3]= {MBL_MW_MODULE_EVENT, ORDINAL(EventRegister::REMOVE)};

        for(auto it: event_command_ids) {
            command[2]= it;
            SEND_COMMAND_BOARD(owner);
        }
    }
}

void mbl_mw_event_record_commands(MblMwEvent *event) {
    event->owner->event_owner= event;
    event->num_expected_cmds= 0;
    event->owner->event_config.assign({event->header.module_id, event->header.register_id, event->header.data_id});
}

void mbl_mw_event_end_record(MblMwEvent *event, MblMwFnVoid commands_recorded) {
    event->owner->event_recorded_callback= commands_recorded;
    event->owner->event_config.clear();

    if (event == event->owner->event_owner) {
        for(auto it: event->commands) {
            send_command(event->owner, it.data(), (uint8_t) it.size());
        }
    }
}

MblMwMetaWearBoard* mbl_mw_event_get_owner(const MblMwEvent *event) {
    return event->owner;
}
