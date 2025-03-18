/**
 * @file command_processor.c
 * @brief Command processing module for handling user inputs.
 *
 * This file implements the core command processing logic, including
 * functions for recognizing commands and executing corresponding handlers.
 * The module currently supports commands like "echo", "LED ON", "LED OFF", and "hexdump".
 *
 * @date 13 November 2024
 * @author Lokesh Senthil Kumar
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "command_processor.h"
#include "led.h"

#define MAX_BUFFER_SIZE 128

// Structure for storing command name and handler function
typedef struct {
    const char *command;
    void (*handler)(const char *input);
} Command;

// Command table containing supported commands and their handlers
const Command command_table[] = {
    {"LED ON", led_on_command},
    {"LED OFF", led_off_command},
    {NULL, NULL}  // End of table marker
};

/**
 * @brief Processes the user input command by trimming extra spaces and tokenizing the input.
 *
 * This function first removes any extra spaces from the input string,
 * tokenizes it to get the main command, and then checks against the command table.
 *
 * @param input The raw user input string.
 */
void process_command(const char *input) {

	//Tokenization for removing extra spaces
    const char *message = input;
	char output[MAX_BUFFER_SIZE] = {0};
	int out_index = 0;
	int in_word = 0;

	// preserve word boundaries
	for (int i = 0; message[i] != '\0'; i++) {
		if (!isspace((unsigned char)message[i])) {
			output[out_index++] = message[i];
			in_word = 1;
		} else if (in_word) {
			output[out_index++] = ' ';
			in_word = 0;
		}
	}

	// Remove trailing space if present
	if (out_index > 0 && output[out_index - 1] == ' ') {
		output[out_index - 1] = '\0';
	} else {
		output[out_index] = '\0';
	}

    for (int i = 0; command_table[i].command != NULL; i++) {
        // Compare input string with each command in the table (case-insensitive)
        if (strncasecmp(output, command_table[i].command, strlen(command_table[i].command)) == 0) {
            command_table[i].handler(output);
            return;
        }
    }
    // Print error if the command is unknown
    printf("Unknown command(%s)\r\n", output);
}

void led_on_command(const char *input) {
    LED_On();
}

/**
 * @brief Handler for the "LED OFF" command.
 *
 * This function turns off the LED by calling the `LED_Off` function.
 *
 * @param input The user input string (not used in this handler).
 */
void led_off_command(const char *input) {
    LED_Off();
}
