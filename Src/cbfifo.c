/**
 * @file cbfifo.c
 * @brief Circular buffer implementation for enqueue and dequeue operations.
 *
 * This file provides functions for handling circular buffers,
 * used for efficient non-blocking I/O operations in USART communication.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#include "cbfifo.h"

/**
 * @brief Enqueues data into a circular buffer.
 *
 * Adds a byte of data to the circular buffer. If the buffer is full,
 * the function returns 0 indicating failure.
 *
 * @param buffer Pointer to the circular buffer array.
 * @param head Pointer to the head index of the buffer.
 * @param tail Pointer to the tail index of the buffer.
 * @param data Data byte to enqueue.
 * @return int Returns 1 on success, 0 if the buffer is full.
 */
int cbfifo_enqueue(uint8_t *buffer, volatile int *head, volatile int *tail, uint8_t data) {
    int next = (*head + 1) % MAX_BUFFER_SIZE;
    if (next == *tail) {
        return 0; // Buffer is full
    }

    buffer[*head] = data;
    *head = next;
    return 1;
}

/**
 * @brief Dequeues data from a circular buffer.
 *
 * Removes a byte of data from the circular buffer. If the buffer is empty,
 * the function returns 0 indicating failure.
 *
 * @param buffer Pointer to the circular buffer array.
 * @param head Pointer to the head index of the buffer.
 * @param tail Pointer to the tail index of the buffer.
 * @param data Pointer to store the dequeued data byte.
 * @return int Returns 1 on success, 0 if the buffer is empty.
 */
int cbfifo_dequeue(uint8_t *buffer, volatile int *head, volatile int *tail, uint8_t *data) {
    if (*head == *tail) {
        return 0; // Buffer is empty
    }

    *data = buffer[*tail];
    *tail = (*tail + 1) % MAX_BUFFER_SIZE;
    return 1;
}
