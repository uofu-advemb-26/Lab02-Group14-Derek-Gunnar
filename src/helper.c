#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"
#include "helper.h"

int count = 0;
bool on = false;
char cha[100];

#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define PRINTF_TASK_PRIORITY    ( tskIDLE_PRIORITY + 3UL )
#define PRINTF_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


void printf_task(__unused void *params) {
    char* c = "hello world!\n";
    int i = 0;
    while(c[i] != '\0') {
        if (c[i] <= 'z' && c[i] >= 'a') {
            cha[i] = c[i] - 32;
            putchar(c[i] - 32);
        } 
        else if (c[i] >= 'A' && c[i] <= 'Z') {
            cha[i] = c[i] + 32;
            putchar(c[i] + 32);
        }
        else {
            cha[i] = c[i];
            putchar(c[i]);
        }
        i++;
    }
    cha[i] = '\0';
}

void blink_task(__unused void *params) {
    int count_2 = 0;
    // this causes it to crash when already initialized
    // hard_assert(cyw43_arch_init() == PICO_OK); 
    while (count_2 < 5) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on;
        sleep_ms(500); //vdelay causes it to crash when it is in the testing phase
        count_2++;

    }
}

void main_task(__unused void *params) {
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    xTaskCreate(printf_task, "PrintfThread",
                PRINTF_TASK_STACK_SIZE, NULL, PRINTF_TASK_PRIORITY, NULL);    
    char c;
    while(c = getchar()) {
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);
    }
}

