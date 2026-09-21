#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "helper.h"

extern char cha[100];
void setUp(void) {
    cha[0] = '\0';
}

void tearDown(void) {}

void test_array_check(void)
{
    int* x = malloc(sizeof(int)*2);
    x[0] = 1;
    x[1] = 2;
    int z = x[0] + x[1];
    TEST_ASSERT_TRUE_MESSAGE(z == 3, "array check FAILED");
    free(x);
}

void test_printf_task(void)
{
    printf_task(NULL);
    TEST_ASSERT_TRUE_MESSAGE(cha[0] == 'H', "printf_task test FAILED");
}

void test_blink_task(void)
{
    blink_task(NULL);
    TEST_ASSERT_TRUE_MESSAGE(1 == 1, "blink_task test FAILED");
}

int main (void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_array_check);
        RUN_TEST(test_printf_task);
        RUN_TEST(test_blink_task);
        sleep_ms(5000);
        UNITY_END();
    }
}