/**
 * @file queue.h
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
#ifndef UTILS_QUEUE_QUEUE_H_
#define UTILS_QUEUE_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct queue_s {
  int *array;
  size_t wr_pos;
  size_t rd_pos;
  size_t size;
  size_t capacity;
} queue_t;

#define QUEUE_DEF(name, capacity)                                              \
  int #name##_array[capacity] = {0};                                           \
  queue_t name = {                                                             \
      .array = #name##_array,                                                  \
      .wr_pos = 0,                                                             \
      .rd_pos = 0,                                                             \
      .size = 0,                                                               \
      .capacity = capacity,                                                    \
  };

bool queue_is_empty(const queue_t *const);

bool queue_is_full(const queue_t *const);

size_t queue_size(const queue_t *const);

size_t queue_capacity(const queue_t *const);

int queue_put(queue_t *, int);

int queue_get(queue_t *, int *);

int queue_peek(queue_t *, int *);

void queue_wrap(queue_t *, int *array, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif // UTILS_QUEUE_QUEUE_H_
