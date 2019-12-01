/**
 * @file queue.c
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
#include "queue.h"

#define queue_lock(queue)
#define queue_unlock(queue)

static bool _queue_is_empty(const queue_t *const queue) {
  return (queue->size == 0);
}

bool queue_is_empty(const queue_t *const queue) {
  queue_lock(queue);
  bool is_empty = _queue_is_empty(queue);
  queue_unlock(queue);
  return is_empty;
}

static bool _queue_is_full(const queue_t *const queue) {
  return (queue->size == queue->capacity);
}

bool queue_is_full(const queue_t *const queue) {
  queue_lock(queue);
  bool is_full = _queue_is_full(queue);
  queue_unlock(queue);
  return is_full;
}

size_t queue_size(const queue_t *const queue) { return queue->size; }

size_t queue_capacity(const queue_t *const queue) { return queue->capacity; }

int queue_put(queue_t *queue, int value) {

  int res = -1;

  queue_lock(queue);

  if (!_queue_is_full(queue)) {
    queue->array[queue->wr_pos] = value;
    queue->wr_pos = (queue->wr_pos + 1) % queue->capacity;
    queue->size++;
    res = 0;
  }

  queue_unlock(queue);

  return res;
}

int queue_get(queue_t *queue, int *pValue) {
  int res = -1;

  queue_lock(queue);

  if (!_queue_is_empty(queue)) {
    *pValue = queue->array[queue->rd_pos];
    queue->rd_pos = (queue->rd_pos + 1) % queue->capacity;
    queue->size--;
    res = 0;
  }

  queue_unlock(queue);

  return res;
}

int queue_peek(queue_t *queue, int *pValue) {
  int res = -1;

  queue_lock(queue);

  if (!_queue_is_empty(queue)) {
    *pValue = queue->array[queue->rd_pos];
    res = 0;
  }

  queue_unlock(queue);

  return res;
}

void queue_wrap(queue_t *queue, int *array, size_t capacity) {
  queue->array = array;
  queue->capacity = capacity;
  queue->size = 0;
  queue->wr_pos = 0;
  queue->rd_pos = 0;
}
