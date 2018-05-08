#include "metawear/core/event.h"
#include "metawear/core/module.h"
#include "metawear/core/status.h"
#include "metawear/core/timer.h"
#include "metawear/platform/cpp/async_creator.h"
#include "metawear/platform/cpp/threadpool.h"

#include "metawearboard_def.h"
#include "timer_private.h"
#include "timer_register.h"

#include <cstring>

using std::forward_as_tuple;
using std::make_shared;
using std::memcpy;
using std::piecewise_construct;
using std::static_pointer_cast;

#define GET_TIMER_STATE(board) static_pointer_cast<TimerState>(board->timer_state)

const uint16_t REPEAT_INDEFINITELY= 0xffff;

struct TimerState : public AsyncCreator {
    MblMwFnTimerPtr timer_callback;
    void *timer_context;
};

static int32_t timer_created(MblMwMetaWearBoard *board, const uint8_t *response, uint8_t len) {
    auto state = GET_TIMER_STATE(board);

    state->timeout->cancel();

    MblMwTimer *new_timer = new MblMwTimer(ResponseHeader(MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::NOTIFY), response[2]), board);
    board->module_events.emplace(new_timer->header, new_timer);

    state->timer_callback(state->timer_context, new_timer);
    state->create_next(true);

    return MBL_MW_STATUS_OK;
}


MblMwTimer::MblMwTimer(uint8_t** state_stream, MblMwMetaWearBoard *owner) : MblMwEvent(state_stream, owner) {
}

MblMwTimer::MblMwTimer(const ResponseHeader& header, MblMwMetaWearBoard *owner) : MblMwEvent(header, owner) {
}

MblMwTimer::~MblMwTimer() {
    if (remove) {
        remove_from_board();
    }
}

void MblMwTimer::remove_from_board() {
    uint8_t command[3] = { MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::REMOVE), header.data_id };
    SEND_COMMAND_BOARD(owner);
}

void init_timer_module(MblMwMetaWearBoard *board) {
    board->responses.emplace(piecewise_construct, forward_as_tuple(MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::TIMER_ENTRY)),
        forward_as_tuple(timer_created));

    if (!board->timer_state) {
        board->timer_state= make_shared<TimerState>();
    }
}

void free_timer_module(void *state) {
    delete (TimerState*) state;
}

void disconnect_timer(MblMwMetaWearBoard* board) {
    auto state = GET_TIMER_STATE(board);
    if (state != nullptr) {
        state->pending_fns.clear();
    }
}

void mbl_mw_timer_create(MblMwMetaWearBoard *board, uint32_t period, uint16_t repetitions, uint8_t delay, void *context, MblMwFnTimerPtr received_timer) {
    auto state = GET_TIMER_STATE(board);

    state->pending_fns.push([=](void) -> void {
        uint8_t command[9]= {MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::TIMER_ENTRY)};
        memcpy(command + 2, &period, sizeof(period));
        memcpy(command + 6, &repetitions, sizeof(repetitions));
        command[8]= (delay != 0) ? 0 : 1;    

        state->timer_callback= received_timer;
        state->timer_context= context;
        state->timeout= ThreadPool::schedule([context, state, received_timer](void) -> void {
            received_timer(context, nullptr);
            state->create_next(true);
        }, board->time_per_response);

        SEND_COMMAND;
    });
    state->create_next(false);
}

void mbl_mw_timer_create_indefinite(MblMwMetaWearBoard *board, uint32_t period, uint8_t delay, void *context, MblMwFnTimerPtr received_timer) {
    return mbl_mw_timer_create(board, period, REPEAT_INDEFINITELY, delay, context, received_timer);
}


uint8_t mbl_mw_timer_get_id(const MblMwTimer* timer) {
    return timer->header.data_id;
}

MblMwTimer* mbl_mw_timer_lookup_id(const MblMwMetaWearBoard* board, uint8_t id) {
    ResponseHeader map_key(MBL_MW_MODULE_TIMER, ORDINAL(TimerRegister::NOTIFY), id);
    return board->module_events.count(map_key) ? dynamic_cast<MblMwTimer*>(board->module_events.at(map_key)) : nullptr;
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
    timer->owner->module_events.erase(timer->header);
    delete timer;
}
