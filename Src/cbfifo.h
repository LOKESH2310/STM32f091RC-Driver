/**
 * @file cbfifo.h
 * @brief Header file for circular buffer functions.
 *
 * This file contains function declarations for enqueue and dequeue operations
 * on circular buffers used in USART communication.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#ifndef CBFIFO_H
#define CBFIFO_H

#include <stdint.h>

#define MAX_BUFFER_SIZE 128 /**< Maximum size for the circular buffer */

/**
 * @brief Enqueues data into a circular buffer.
 *
 * @param buffer Pointer to the circular buffer array.
 * @param head Pointer to the head index of the buffer.
 * @param tail Pointer to the tail index of the buffer.
 * @param data Data byte to enqueue.
 * @return int Returns 1 on success, 0 if the buffer is full.
 */
int cbfifo_enqueue(uint8_t *buffer, volatile int *head, volatile int *tail, uint8_t data);

/**
 * @brief Dequeues data from a circular buffer.
 *
 * @param buffer Pointer to the circular buffer array.
 * @param head Pointer to the head index of the buffer.
 * @param tail Pointer to the tail index of the buffer.
 * @param data Pointer to store the dequeued data byte.
 * @return int Returns 1 on success, 0 if the buffer is empty.
 */
int cbfifo_dequeue(uint8_t *buffer, volatile int *head, volatile int *tail, uint8_t *data);

#endif // CBFIFO_H
