/**
 * @file USART.c
 * @brief UART communication module for STM32F091RC microcontroller.
 *
 * This module provides initialization and interrupt-driven UART communication
 * using USART2. It includes circular buffers for RX (receive) and TX (transmit)
 * data handling to ensure non-blocking I/O operations.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#include "stm32f0xx.h"
#include "USART.h"
#include "cbfifo.h"

#define MAX_BUFFER_SIZE 128 /**< Maximum size for RX and TX circular buffers */
#define AF_Mode_PA2_PA3_clear ((3U << (2 * 2)) | (3U << (2 * 3)))
#define AF_Mode_PA2_PA3_set (2U << (2 * 2)) | (2U << (2 * 3))
// Configuration Defines
#define USART_BAUD_RATE   19200         /**< Baud rate for USART communication */
#define USART_DATA_and_parity_BITS   9             /**< Number of data bits and parity bit (8 or 9) */
#define USART_PARITY      'O'           /**< Parity: 'N' (None), 'E' (Even), 'O' (Odd) */
#define USART_STOP_BITS   1             /**< Stop bits: 1 or 2 */
#define peripheral_frequency 24000000


// Circular buffers for RX and TX
static uint8_t rx_buffer[MAX_BUFFER_SIZE];
static uint8_t tx_buffer[MAX_BUFFER_SIZE];
static volatile int rx_head = 0, rx_tail = 0;
static volatile int tx_head = 0, tx_tail = 0;


/**
 * @brief Initializes USART2 peripheral for UART communication.
 *
 * Configures GPIO pins (PA2, PA3) for alternate function mode, sets up USART2
 * for 19200 baud rate, 9-bit data with odd parity, and enables interrupts for
 * RXNE (Receive Not Empty) and TXE (Transmit Empty) events.
 */
void USART2_Init(void) {
    // Enable clock for GPIOA and USART2
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA2 (TX) and PA3 (RX) for USART2 (Alternate Function Mode)
    GPIOA->MODER &= ~AF_Mode_PA2_PA3_clear;
    GPIOA->MODER |= AF_Mode_PA2_PA3_set;
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk);
    GPIOA->AFR[0] |= (1U << GPIO_AFRL_AFSEL2_Pos) | (1U << GPIO_AFRL_AFSEL3_Pos);

    // Configure USART Baud Rate
    USART2->CR1 &= ~USART_CR1_OVER8;  // Use 16x oversampling
    USART2->BRR = (peripheral_frequency / USART_BAUD_RATE);

    // Configure Parity
    if (USART_PARITY == 'N') {
        USART2->CR1 &= ~USART_CR1_PCE; // Disable parity
    } else {
        USART2->CR1 |= USART_CR1_PCE; // Enable parity
        if (USART_PARITY == 'E') {
            USART2->CR1 &= ~USART_CR1_PS; // Even parity
        } else if (USART_PARITY == 'O') {
            USART2->CR1 |= USART_CR1_PS; // Odd parity
        }
    }

    // Configure Data Bits
    if (USART_DATA_and_parity_BITS == 9) {
        USART2->CR1 |= USART_CR1_M0;   // 9 data bits
        USART2->CR1 &= ~USART_CR1_M1;
    } else {
        USART2->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1); // 8 data bits
    }

    // Configure Stop Bits
    if (USART_STOP_BITS == 2) {
        USART2->CR2 |= USART_CR2_STOP_1; // 2 stop bits
    } else {
        USART2->CR2 &= ~USART_CR2_STOP;  // 1 stop bit
    }

    // Enable transmitter, receiver, and USART module
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    // Enable RXNE and TXE interrupts
    USART2->CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;

    // Enable USART2 interrupt in the NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * @brief Sends a character via USART2 (used by printf).
 *
 * @param ch Character to send.
 * @return int Returns the sent character.
 */
int __io_putchar(int ch) {
    cbfifo_enqueue(tx_buffer, &tx_head, &tx_tail, (uint8_t)ch);
    USART2->CR1 |= USART_CR1_TXEIE; // Enable TXE interrupt
    return ch;
}

/**
 * @brief Receives a character via USART2 (used by getchar).
 *
 * @return int Returns the received character.
 */
int __io_getchar(void) {
    uint8_t ch;
    while (cbfifo_dequeue(rx_buffer, &rx_head, &rx_tail, &ch) == 0);
    return ch;
}

/**
 * @brief Standard putchar implementation for UART output.
 *
 * @param ch Character to send.
 * @return int Returns the sent character.
 */
int putchar(int ch) {
    return __io_putchar(ch);
}

/**
 * @brief Standard getchar implementation for UART input.
 *
 * @return int Returns the received character.
 */
int getchar(void) {
    return __io_getchar();
}

/**
 * @brief USART2 interrupt handler.
 *
 * Handles RXNE (Receive Not Empty) and TXE (Transmit Empty) interrupts.
 * RXNE: Reads received data and stores it in the RX circular buffer.
 * TXE: Sends data from the TX circular buffer if available, otherwise disables TXE interrupt.
 */
void USART2_IRQHandler(void) {
    uint8_t byte;

    // Handle RXNE interrupt (data received)
    if (USART2->ISR & USART_ISR_RXNE) {
        byte = (uint8_t)(USART2->RDR & 0xFF); // Read received data
        cbfifo_enqueue(rx_buffer, &rx_head, &rx_tail, byte);
    }

    // Handle TXE interrupt (transmit buffer empty)
    if ((USART2->CR1 & USART_CR1_TXEIE) && (USART2->ISR & USART_ISR_TXE)) {
        if (cbfifo_dequeue(tx_buffer, &tx_head, &tx_tail, &byte)) {
            USART2->TDR = byte; // Write data to transmit data register
        } else {
            USART2->CR1 &= ~USART_CR1_TXEIE; // Disable TXE interrupt if buffer is empty
        }
    }
}


