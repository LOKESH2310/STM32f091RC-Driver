/**
 * @file LED.c
 * @brief LED control functions for STM32F0 microcontrollers.
 *
 * This file contains the implementation of functions to initialize,
 * turn on, and turn off the LED connected to pin PA5 on the STM32F0.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#include "stm32f0xx.h"
#include "LED.h"

#define led_outmode_clear (3U << (5 * 2))
#define led_outmode_set (1U << (5 * 2))
/**
 * @brief Initializes the LED on PA5.
 *
 * Configures the GPIOA peripheral clock and sets the PA5 pin as an output.
 * This function is called once during system initialization.
 */
void LED_Init(void) {
    // Enable the clock for GPIOA (AHB Bus)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~led_outmode_clear; // 3U (0b11) is used to clear both bits
    // Set PA5 to output mode (01)
    GPIOA->MODER |= led_outmode_set; // 1U (0b01) sets the pin to output mode
}

/**
 * @brief Turns on the LED connected to PA5.
 *
 * This function sets the PA5 pin high, turning on the LED.
 */
void LED_On(void) {
    // Set the bit for PA5 in the GPIOA BSRR register
    GPIOA->BSRR = GPIO_BSRR_BS_5;
}

/**
 * @brief Turns off the LED connected to PA5.
 *
 * This function resets the PA5 pin, turning off the LED.
 */
void LED_Off(void) {
    // Reset the bit for PA5 in the GPIOA BSRR register
    GPIOA->BSRR = GPIO_BSRR_BR_5;
}
