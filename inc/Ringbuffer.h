#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef RINGBUFFER_LENGTH
#define RINGBUFFER_LENGTH 1024U
// #define RINGBUFFER_LENGTH 8U
#endif

#if ((RINGBUFFER_LENGTH & (RINGBUFFER_LENGTH - 1)) != 0)
#error "Definition RINGBUFFER_LENGTH must be power-of-two "
#endif
/*
 * Modulo:
 * X & (RINGBUFFER_LENGTH - 1) = X % RINGBUFFER_LENGTH (faster)
 *
 * Indexing:
 * The end and the start variables have a range of 0 .. 2*RINGBUFFER_LENGTH.
 * This helps detect full and empty buffers and prevents overflow in these variables.
 *
 */

typedef struct ringbuffer_s
{
    uint32_t buffer[RINGBUFFER_LENGTH];
    uint32_t end;
    uint32_t start;
} ringbuffer_t;

void ringbufferInit(void);

void addValue(uint32_t a_value);
uint32_t* getValues(uint32_t a_length);

uint32_t getEnd(void);
uint32_t getStart(void);
uint32_t getSize(void);

bool isEmpty(void);
bool isFull(void);

bool isValid(void);

#ifdef __cplusplus
}
#endif

#endif // !RINGBUFFER_H_