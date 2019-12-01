/**
 * @file fsm_ex.c
 * @author Ahmed Zamouche (ahmed.zamouche@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-12-01
 *
 *  @copyright Copyright (c) 2019
 *
 * MIT License
 *
 * Copyright (c) 2019 Ahmed Zamouche
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "fsm_ex.h"

#include <stddef.h>

typedef enum ex_fsm_event_e {
  EX_FSM_EVENT_0 = 0,
  EX_FSM_EVENT_1,
  EX_FSM_EVENT_2,
  EX_FSM_EVENT_3,

  EX_FSM_EVENT_NUM,
} ex_fsm_event_t;

typedef enum ex_fsm_state_e {
  EX_FSM_STATE_0 = 0,
  EX_FSM_STATE_1,
  EX_FSM_STATE_2,
  EX_FSM_STATE_3,

  EX_FSM_STATE_NUM,
} ex_fsm_state_t;

static const fsm_event_t ex_events[EX_FSM_EVENT_NUM];
static const fsm_state_t ex_states[EX_FSM_STATE_NUM];

static void ex_s0_on_entry(void) {}
static void ex_s0_on_exit(void) {}
static const fsm_state_t *ex_s0_guard(const fsm_event_t *event) {

  if (event->id == EX_FSM_EVENT_1) {
    return &ex_states[EX_FSM_STATE_1];
  }
  return NULL;
}

static void ex_s1_on_entry(void) {}
static void ex_s1_on_exit(void) {}
static const fsm_state_t *ex_s1_guard(const fsm_event_t *event) {

  if (event->id == EX_FSM_EVENT_2) {
    return &ex_states[EX_FSM_STATE_2];
  } else if (event->id == EX_FSM_EVENT_1) {
    return &ex_states[EX_FSM_STATE_1];
  }
  return NULL;
}

static void ex_s2_on_entry(void) {}
static void ex_s2_on_exit(void) {}
static const fsm_state_t *ex_s2_guard(const fsm_event_t *event) {

  if (event->id == EX_FSM_EVENT_3) {
    return &ex_states[EX_FSM_STATE_3];
  }
  return NULL;
}

static void ex_s3_on_entry(void) {}
static void ex_s3_on_exit(void) {}
static const fsm_state_t *ex_s3_guard(const fsm_event_t *event) {

  if (event->id == EX_FSM_EVENT_0) {
    return &ex_states[EX_FSM_STATE_0];
  } else if (event->id == EX_FSM_EVENT_2) {
    return (const fsm_state_t *)&FSM_TERMINATE_STATE;
  }
  return NULL;
}

static const fsm_event_t ex_events[] = {
    {
        .id = EX_FSM_EVENT_0,
        .name = "Event_0",
    },
    {
        .id = EX_FSM_EVENT_1,
        .name = "Event_1",
    },
    {
        .id = EX_FSM_EVENT_2,
        .name = "Event_2",
    },
    {
        .id = EX_FSM_EVENT_3,
        .name = "Event_3",
    },
};

const fsm_event_list_t ex_event_list = {.length = ARRAY_SIZE(ex_events),
                                        .events = ex_events};

static const fsm_state_t ex_states[EX_FSM_STATE_NUM] = {
    {
        .id = EX_FSM_STATE_0,
        .name = "State_0",
        .on_entry = ex_s0_on_entry,
        .on_exit = ex_s0_on_exit,
        .transition = {.name = "Guard_0", .guard = ex_s0_guard},
    },
    {
        .id = EX_FSM_STATE_1,
        .name = "State_1",
        .on_entry = ex_s1_on_entry,
        .on_exit = ex_s1_on_exit,
        .transition = {.name = "Guard_1", .guard = ex_s1_guard},
    },
    {
        .id = EX_FSM_STATE_2,
        .name = "State_2",
        .on_entry = ex_s2_on_entry,
        .on_exit = ex_s2_on_exit,
        .transition = {.name = "Guard_2", .guard = ex_s2_guard},
    },
    {
        .id = EX_FSM_STATE_3,
        .name = "State_3",
        .on_entry = ex_s3_on_entry,
        .on_exit = ex_s3_on_exit,
        .transition = {.name = "Guard_3", .guard = ex_s3_guard},
    },
};

const fsm_state_list_t ex_state_list = {.length = ARRAY_SIZE(ex_states),
                                        .states = ex_states};
