/*
 * Copyright (c) 2017, 2018, Oracle and/or its affiliates.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <polyglot.h>

struct Point {
  int x;
  int y;
};

POLYGLOT_DECLARE_STRUCT(Point)

int distSquared(void *a, void *b) {
  int distX = polyglot_as_Point(b)->x - polyglot_as_Point(a)->x;
  int distY = polyglot_as_Point(b)->y - polyglot_as_Point(a)->y;
  return distX * distX + distY * distY;
}

void flipPoint(void *value) {
  struct Point *point = polyglot_as_Point(value);
  int tmp = point->x;
  point->x = point->y;
  point->y = tmp;
}

int sumPoints(void *pointArray) {
  int sum;

  struct Point *arr = polyglot_as_Point_array(pointArray);
  int len = polyglot_get_array_size(pointArray);
  for (int i = 0; i < len; i++) {
    sum += arr[i].x + arr[i].y;
  }

  return sum;
}

void fillPoints(void *pointArray, int x, int y) {
  struct Point *arr = polyglot_as_Point_array(pointArray);
  int len = polyglot_get_array_size(pointArray);

  for (int i = 0; i < len; i++) {
    arr[i].x = x;
    arr[i].y = y;
  }
}

struct Nested {
  struct Point arr[5];
  struct Point direct;
  struct Nested *next;
};

POLYGLOT_DECLARE_STRUCT(Nested)

void fillNested(void *arg) {
  int value = 42;

  struct Nested *nested = polyglot_as_Nested(arg);
  while (nested) {
    for (int i = 0; i < 5; i++) {
      nested->arr[i].x = value++;
      nested->arr[i].y = value++;
    }
    nested->direct.x = value++;
    nested->direct.y = value++;

    nested = nested->next;
  }
}

struct BitFields {
  int x : 4;
  int y : 3;
  int z;
};

POLYGLOT_DECLARE_STRUCT(BitFields)

int accessBitFields(void *arg) {
	struct BitFields *obj = polyglot_as_BitFields(arg);
	return obj->x + obj->y + obj->z;
}
