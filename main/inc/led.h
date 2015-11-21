#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdbool.h>

void led_init(void);
void set_led_blue(bool status);
void set_led_red(bool status);
void set_led_orange(bool status);
void set_led_green(bool status);

#endif // __GPIO_H__