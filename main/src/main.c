#include "driver_init.h"

#include "led.h"

int main(void)
{
    driver_init();

    while(1) {
        set_led_blue(true);
    }

    return 0;
}