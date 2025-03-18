/**
 * @file command_processor.h
 * @brief Header file for the command processing module.
 *
 * This module provides function declarations for handling user input commands.
 * It includes functions for processing commands, echoing input, and controlling the LED.
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <stdint.h>

// Function Declarations
void process_command(const char *input);
void echo_command(const char *input);
void led_on_command(const char *input);
void led_off_command(const char *input);
void normalize_input(const char *input, char *output);

#endif // COMMAND_PROCESSOR_H
