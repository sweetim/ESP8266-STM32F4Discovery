#include "driver_init.h"

#include "stm32f4xx.h"

#include "led.h"
#include "usb_vcp.h"

static void init_driver(void);

void driver_init(void)
{
    init_driver();
}

static void init_driver(void)
{
    init_led();

    TM_USB_VCP_Init();
}