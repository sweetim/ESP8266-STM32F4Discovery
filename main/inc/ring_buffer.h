/**
 * ring_buffer.h
 *
 * Copyright (c) 2011 ZMP Inc. All rights reserved.
 */

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include <stdbool.h>

typedef struct {
	int head;
	int tail;
	int size;
	char *buffer;
} RingBuffer;

RingBuffer *ring_buffer_create(int size);
void ring_buffer_destroy(RingBuffer *instance);
bool ring_buffer_put(RingBuffer *instance, const char *buf, int size);
bool ring_buffer_put_1(RingBuffer *instance, const char data);
bool ring_buffer_get(RingBuffer *instance, char *buf, int size);
bool ring_buffer_get_1(RingBuffer *instance, char *data);
void ring_buffer_empty(RingBuffer *instance);
int ring_buffer_queue_length(RingBuffer *instance);
char ring_buffer_peek_last(RingBuffer *instance);

#endif

