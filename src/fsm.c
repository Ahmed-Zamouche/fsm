/**
 * @file fsm.c
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
#include "fsm.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const fsm_pseudo_state_t FSM_INITIAL_STATE = {
    .id = -1,
    .name = "Initial",
};
const fsm_pseudo_state_t FSM_TERMINATE_STATE = {
    .id = -1,
    .name = "Terminate",
};

/**
 * @brief default final state callback function if none is registred
 *
 */
static void fsm_final_state_default_cb(void) {}

static void fsm_print_states(fsm_t *fsm, FILE *stream, const fsm_state_t *state,
                             int *ids) {

  (void)fsm;
  if (ids[state->id]) {
    // state already visited;
    return;
  }

  ids[state->id] = 1;
  for (size_t i = 0; i < fsm->event_list->length; i++) {

    const fsm_event_t *event = &fsm->event_list->events[i];

    const fsm_state_t *nxt_state = state->transition.guard(event);
    if (nxt_state == (const fsm_state_t *)&FSM_TERMINATE_STATE) {
      fprintf(stream, "   %s --> [*]: %s[%s]\r\n", state->name, event->name,
              state->transition.name);
    } else if (nxt_state) {
      //--Trigger[Guard]/Effect-->
      fprintf(stream, "    %s --> %s : %s[%s]\r\n", state->name,
              nxt_state->name, event->name, state->transition.name);
      fsm_print_states(fsm, stream, nxt_state, ids);
    } else {
      ;
    }
  }
}

int fsm_event_put(fsm_t *fsm, const fsm_event_t *event) {
  return queue_put(&fsm->queue, event->id) ? -1 : event->id;
}

void fsm_print(fsm_t *fsm, FILE *stream) {

  int *ids = (int *)malloc(fsm->state_list->length * sizeof(int));

  assert(ids != NULL);

  memset(ids, 0, fsm->state_list->length * sizeof(int));

  fprintf(stream, "@startuml\r\n");
  fprintf(stream, "    title `%s` Finate State Machine\r\n", fsm->name);
  fprintf(stream, "    [*] -> %s\r\n", fsm->init_state->name);

  fsm_print_states(fsm, stream, fsm->init_state, ids);

  fprintf(stream, "@enduml\r\n");

  free(ids);
}

void fsm_register_final_state_callback(fsm_t *fsm,
                                       void (*final_state_cb)(void)) {
  fsm->final_state_cb =
      final_state_cb ? final_state_cb : fsm_final_state_default_cb;
}

void fsm_mainloop(fsm_t *fsm) {

  int event_id;

  if (fsm->cur_state == (const fsm_state_t *)&FSM_INITIAL_STATE) {
    fprintf(stderr, "info: fsm `%s`, %s state\r\n", fsm->name,
            FSM_INITIAL_STATE.name);
    fsm->cur_state = fsm->init_state;
    if (fsm->cur_state->on_entry) {
      fsm->cur_state->on_entry();
    }
  }

  while (!queue_get(&fsm->queue, &event_id)) {

    if ((size_t)event_id >= fsm->event_list->length) {
      fprintf(stderr, "warning: fsm `%s`, unknown event id %d\r\n", fsm->name,
              event_id);
      continue;
    }

    const fsm_event_t *event = &fsm->event_list->events[event_id];

    fprintf(stderr, "info: fsm `%s`, received event `%s`\r\n", fsm->name,
            event->name);

    if (fsm->cur_state == (const fsm_state_t *)&FSM_TERMINATE_STATE) {
      return;
    }

    const fsm_state_t *nxt_state = fsm->cur_state->transition.guard(event);

    if (nxt_state == NULL) {
      continue;
    }

    fprintf(stderr, "info: fsm `%s`, %s -[%s]-> %s\r\n", fsm->name,
            fsm->cur_state->name, event->name, nxt_state->name);

    if (fsm->cur_state->on_exit) {
      fsm->cur_state->on_exit();
    }

    fsm->cur_state = nxt_state;

    if (fsm->cur_state == (const fsm_state_t *)&FSM_TERMINATE_STATE) {
      fprintf(stderr, "info: fsm `%s`, %s state\r\n", fsm->name,
              FSM_TERMINATE_STATE.name);
      fsm->final_state_cb();
      return;
    }

    if (fsm->cur_state->on_entry) {
      fsm->cur_state->on_entry();
    }
  }
}

void fsm_init(fsm_t *fsm, const char *name, const fsm_state_list_t *state_list,
              const fsm_state_t *init_state,
              const fsm_event_list_t *event_list) {

  fsm->name = name;

  fsm->cur_state = (const fsm_state_t *)&FSM_INITIAL_STATE;

  fsm->init_state = init_state ? init_state : &state_list->states[0];

  fsm->state_list = state_list;

  fsm->event_list = event_list;

  fsm->final_state_cb = fsm_final_state_default_cb;

  queue_wrap(&fsm->queue, fsm->queue_buf, ARRAY_SIZE(fsm->queue_buf));
}
