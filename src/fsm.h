/**
 * @file fsm.h
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
#ifndef _FSM_H
#define _FSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "queue.h"

#include <stdbool.h>
#include <stdio.h>

#ifndef FSM_EVENT_QUEUE_SIZE
#define FSM_EVENT_QUEUE_SIZE (8) /**< */
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

/**
 * @brief
 *
 */
typedef struct fsm_event_s {
  int id;           /**< */
  const char *name; /**< */
} fsm_event_t;

/**
 * @brief
 *
 */
typedef struct fsm_event_list_s {
  size_t length;             /**< */
  const fsm_event_t *events; /**< */
} fsm_event_list_t;

/**
 * @brief
 * Definition of the finite state machine struct
 */
typedef struct fsm_s fsm_t;

/**
 * @brief
 *
 */
typedef struct fsm_state_s fsm_state_t;

/**
 * @brief
 *
 */
typedef struct transition_s {
  const char *name;                                 /**< */
  const fsm_state_t *(*guard)(const fsm_event_t *); /**< */
} transition_t;

/**
 * @brief
 *
 */
typedef struct fsm_pseudo_state_s {
  int id;           /**< */
  const char *name; /**< */
} fsm_pseudo_state_t;

/**
 * @brief
 *
 */
typedef struct fsm_state_s {

  int id;                 /**< */
  const char *name;       /**< */
  void (*on_entry)(void); /**< action performed upon entry to the state*/
  // void (*on_going)(void);  /**< action performed as long as in the state*/
  void (*on_exit)(void);   /**< action performed upon exit from the state*/
  transition_t transition; /**< */
} fsm_state_t;

/**
 * @brief
 *
 */
typedef struct fsm_state_list_s {
  size_t length;             /**< */
  const fsm_state_t *states; /**< */
} fsm_state_list_t;

/**
 * @brief Definition of the finite state machine struct
 *
 */
struct fsm_s {
  const char *name;                    /**< */
  const fsm_state_t *cur_state;        /**< */
  const fsm_state_t *init_state;       /**< */
  const fsm_state_list_t *state_list;  /**< */
  const fsm_event_list_t *event_list;  /**< */
  void (*final_state_cb)(void);        /**< */
  int queue_buf[FSM_EVENT_QUEUE_SIZE]; /**< */
  queue_t queue;                       /**< */
};

extern const fsm_pseudo_state_t FSM_TERMINATE_STATE;

/**
 * @brief puts event into internal events queue
 *
 * @param fsm the finate state machine struct
 * @param event the event to put
 * @return int Upon successful completion event is returned.  Otherwise, -1 is
 * returned
 */
int fsm_event_put(fsm_t *fsm, const fsm_event_t *event);

/**
 * @brief
 *
 * @param fsm the finate state machine struct
 * @param stream
 */
void fsm_print(fsm_t *fsm, FILE *stream);

/**
 * @brief
 *
 * @param fsm
 * @param final_state_cb
 */
void fsm_register_final_state_callback(fsm_t *fsm,
                                       void (*final_state_cb)(void));

/**
 * @brief
 * @param fsm the finite state machine struct
 */
void fsm_mainloop(fsm_t *fsm);

/**
 * @brief Initialise finite state machine struct
 *
 * @param fsm the finite state machine struct
 * @param name
 * @param state_list
 * @param init_state
 * @param event_list
 */
void fsm_init(fsm_t *fsm, const char *name, const fsm_state_list_t *state_list,
              const fsm_state_t *init_state,
              const fsm_event_list_t *event_list);

#ifdef __cplusplus
}
#endif

#endif /* _FSM_H */