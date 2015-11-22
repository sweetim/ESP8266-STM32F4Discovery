#include "driver_init.h"

#include "stm32f4xx.h"

#include "led.h"
#include "usb_vcp.h"
#include "uart_2.h"
#include "tim_3.h"
#include "adc_1.h"

static void init_driver(void);

void driver_init(void)
{
    init_driver();
}

static void init_driver(void)
{
    TM_USB_VCP_Init();
    uart_2_init(115200);
    led_init();
    tim_3_init();
    adc_1_init();
}