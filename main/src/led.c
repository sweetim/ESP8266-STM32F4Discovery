#include "led.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define LED_PORT GPIOD
#define LED_GREEN_PIN GPIO_Pin_12
#define LED_ORANGE_PIN GPIO_Pin_13
#define LED_RED_PIN GPIO_Pin_14
#define LED_BLUE_PIN GPIO_Pin_15

static void set_led_pin(GPIO_TypeDef *gpio, uint16_t pin, bool status);

void init_led(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN | LED_ORANGE_PIN | LED_RED_PIN | LED_BLUE_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void set_led_blue(bool status)
{
    set_led_pin(LED_PORT, LED_BLUE_PIN, status);
}

void set_led_red(bool status)
{
    set_led_pin(LED_PORT, LED_RED_PIN, status);
}

void set_led_orange(bool status)
{
    set_led_pin(LED_PORT, LED_ORANGE_PIN, status);
}

void set_led_green(bool status)
{
    set_led_pin(LED_PORT, LED_GREEN_PIN, status);
}

static void set_led_pin(GPIO_TypeDef *gpio, uint16_t pin, bool status)
{
    if (status) {
        GPIO_SetBits(gpio, pin);

        return;
    }

    GPIO_ResetBits(gpio, pin);
}