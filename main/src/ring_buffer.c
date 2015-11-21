#include "ring_buffer.h"

#include <stdlib.h>

RingBuffer *ring_buffer_create(int size)
{
	RingBuffer *ring_buffer = malloc(sizeof(*ring_buffer));

	ring_buffer->size = size;
	ring_buffer->buffer = malloc(size * sizeof(char));
	ring_buffer->head = 0;
	ring_buffer->tail = 0;

	return ring_buffer;
}

void ring_buffer_destroy(RingBuffer *instance)
{
	free(instance->buffer);
	free(instance);
}

bool ring_buffer_put(RingBuffer *instance, const char *buf, int size)
{
	if (ring_buffer_queue_length(instance) + size >= instance->size - 1) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		instance->buffer[instance->tail++] = *buf++;
		instance->tail %= instance->size;
	}

	return true;
}

bool ring_buffer_put_1(RingBuffer *instance, const char data)
{
	if (instance->tail != instance->size - 1) {
		if (instance->tail + 1 != instance->head) {
			instance->buffer[instance->tail++] = data;

			return true;
		}

		return false;
	}

	if (instance->head != 0) {
		instance->buffer[instance->tail] = data;
		instance->tail = 0;

		return true;
	}

	return false;
}

bool ring_buffer_get(RingBuffer *instance, char *buf, int size)
{
	if (ring_buffer_queue_length(instance) < size) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		*buf++ = instance->buffer[instance->head++];
		instance->head %= instance->size;
	}

	return true;
}

bool ring_buffer_get_1(RingBuffer *instance, char *data)
{
	if (instance->head != instance->tail) {
		*data = instance->buffer[instance->head++];

		if (instance->head >= instance->size) {
			instance->head = 0;
		}

		return true;
	}

	return false;
}

void ring_buffer_empty(RingBuffer *instance)
{
	instance->head = 0;
	instance->tail = 0;
}

int ring_buffer_queue_length(RingBuffer *instance)
{
	int length = instance->tail - instance->head;

	if (length < 0)	{
		length += instance->size;
	}

	return length;
}

char ring_buffer_peek_last(RingBuffer *instance)
{
	return instance->buffer[instance->tail - 1];
}