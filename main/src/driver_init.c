#include "driver_init.h"

#include "stm32f4xx.h"

#include "led.h"

static void init_driver(void);

void driver_init(void)
{
    SystemInit();

    init_driver();
}

static void init_driver(void)
{
    init_led();
}