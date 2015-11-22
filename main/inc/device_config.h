#ifndef __DEVICE_CONFIG_H__
#define __DEVICE_CONFIG_H__

#define UART_2_PORT             GPIOA
#define UART_2_TX_PIN           GPIO_Pin_2
#define UART_2_TX_PIN_SOURCE    GPIO_PinSource2
#define UART_2_RX_PIN           GPIO_Pin_3
#define UART_2_RX_PIN_SOURCE    GPIO_PinSource3

#define ADC_1_PORT              GPIOA
#define ADC_1_PIN               GPIO_Pin_1

#define LED_PORT                GPIOD
#define LED_GREEN_PIN           GPIO_Pin_12
#define LED_ORANGE_PIN          GPIO_Pin_13
#define LED_RED_PIN             GPIO_Pin_14
#define LED_BLUE_PIN            GPIO_Pin_15

#define USB_VCP_IRQ_PRIORITY    1
#define TIM_3_IRQ_PRIORITY      2
#define UART_2_IRQ_PRIORITY     3
#define DMA_1_IRQ_PRIORITY      4



#endif // __DEVICE_CONFIG_H__