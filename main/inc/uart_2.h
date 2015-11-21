#ifndef __UART_2_H__
#define __UART_2_H__

#include <stdint.h>
#include <stdbool.h>

void uart_2_init(uint32_t baudrate);
bool uart_2_data_received(void);
int uart_2_get(uint8_t *data);
void uart_2_send(uint8_t *data, uint16_t length);

#ifdef __cplusplus
extern "C" {
#endif

    void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // __UART_1_H__