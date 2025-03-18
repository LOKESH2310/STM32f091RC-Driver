/**
 * @file USART.h
 * @brief Header file for USART communication module.
 *
 * This file declares functions and macros used for UART communication
 * using the USART2 peripheral on STM32F0 microcontrollers. It includes
 * initialization, I/O functions, and interrupt handler declarations.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#ifndef USART_H
#define USART_H

#include <stdint.h>

// Function Declarations
void USART2_Init(void);
int __io_putchar(int ch);
int __io_getchar(void);
int putchar(int ch);
int getchar(void);
void USART2_IRQHandler(void);

#endif // USART_H
