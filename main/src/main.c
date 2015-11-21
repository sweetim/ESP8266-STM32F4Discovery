#include "driver_init.h"

#include "led.h"
#include "usb_vcp.h"
#include "uart_2.h"

uint8_t rx_buffer[512];

void main(void)
{
    driver_init();

    uint8_t c;

    while(true) {
        if (uart_2_data_received()) {
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