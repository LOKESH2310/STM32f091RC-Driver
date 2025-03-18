/**
 * @file LED.h
 * @brief Header file for LED control functions.
 *
 * This header file provides function declarations for initializing,
 * turning on, and turning off the LED connected to PA5 on the STM32F0.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#ifndef LED_H
#define LED_H

// Function Declarations
void LED_Init(void);
void LED_On(void);
void LED_Off(void);

#endif // LED_H
