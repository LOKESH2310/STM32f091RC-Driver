

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f0xx.h"
#include "usart.h"
#include "led.h"
#include "command_processor.h"
#include "stts22h_reg.h"

int main(void) {
    // Initialize USART2 for serial communication
    USART2_Init();
    // Initialize the GPIO for LED control
    LED_Init();


    printf("$$ Welcome to SerialIO!\r\n");

    while (1) {
        printf("123");

    }
}
