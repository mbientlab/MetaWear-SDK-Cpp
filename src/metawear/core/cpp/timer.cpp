#include "metawear/core/event.h"
#include "metawear/core/timer.h"

#include "metawearboard_def.h"
#include "timer_private.h"
#include "timer_register.h"

#include <cstring>

using std::memcpy;

static const uint16_t REPEAT_INDEFINITELY= 0xffff;

MblMwTimer::MblMwTimer(const ResponseHeader& header, MblMwMetaWearBoard *owner) : MblMwEvent(header, owner) {
}

MblMwTimer::~MblMwTimer() {
    uint8_t command[3]= {MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::REMOVE), header.data_id};
    SEND_COMMAND_BOARD(owner);
}

void mbl_mw_timer_create(MblMwMetaWearBoard *board, uint32_t period, uint16_t repetitions, uint8_t delay, 
        MblMwTimerPtr received_timer) {
    board->timer_callbacks.push(received_timer);

    ResponseHeader timer_header(MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::NOTIFY));
    MblMwTimer *new_timer= new MblMwTimer(timer_header, board);
    board->pending_timers.emplace(new_timer);

    uint8_t command[9]= {MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::TIMER_ENTRY)};
    memcpy(command + 2, &period, sizeof(period));
    memcpy(command + 6, &repetitions, sizeof(repetitions));
    command[8]= (delay != 0) ? 0 : 1;
    SEND_COMMAND;
}

void mbl_mw_timer_create_indefinite(MblMwMetaWearBoard *board, uint32_t period, uint8_t delay, MblMwTimerPtr received_timer) {
    return mbl_mw_timer_create(board, period, REPEAT_INDEFINITELY, delay, received_timer);
}

void mbl_mw_timer_start(const MblMwTimer* timer) {
    uint8_t command[3]= {MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::START), timer->header.data_id};
    SEND_COMMAND_BOARD(timer->owner);
}

void mbl_mw_timer_stop(const MblMwTimer* timer) {
    uint8_t command[3]= {MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::STOP), timer->header.data_id};
    SEND_COMMAND_BOARD(timer->owner);
}

void mbl_mw_timer_remove(MblMwTimer* timer) {
    mbl_mw_timer_stop(timer);
    timer->owner->timer_signals.erase(timer->header);
    delete timer;
}
