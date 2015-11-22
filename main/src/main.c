#include "driver_init.h"

#include <string.h>

#include "led.h"
#include "usb_vcp.h"
#include "uart_2.h"
#include "tim_3.h"
#include "adc_1.h"

uint8_t rx_buffer[512];


void main(void)
{
    bool led_status = false;
    bool led_temperature_status = false;

    driver_init();

    uint8_t c;
    while(true) {
        if (adc_1_is_interupt()) {
            adc_1_clear_interupt();

            set_led_orange(led_temperature_status);
            led_temperature_status = !led_temperature_status;
        }

        if (tim_3_is_interupt()) {
            tim_3_clear_interupt();

            char text[128];
            uint16_t temperature = adc_1_get_value();
            sprintf(text, "%d\r\n", temperature);
            uart_2_send((uint8_t *)text, strlen(text));
            //TM_USB_VCP_Puts(text);

            //uart_2_dma_tx((uint8_t *)text, strlen(text));


            set_led_green(led_status);
            led_status = !led_status;
        }

        if (uart_2_is_interupt()) {
            uart_2_clear_interupt();

            int length = uart_2_get(rx_buffer);

            for (int i = 0; i < length; i++) {
                TM_USB_VCP_Putc(rx_buffer[i]);
            }
        }


        if (TM_USB_VCP_Getc(&c) == TM_USB_VCP_DATA_OK) {
            uart_2_send(&c, 1);
            //TM_USB_VCP_Putc(c);
        }
    }
}