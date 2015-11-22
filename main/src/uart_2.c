#include "uart_2.h"

#include "stm32f4xx_usart.h"

#include "ring_buffer.h"

#define UART_2_PORT             GPIOA
#define UART_2_TX_PIN           GPIO_Pin_2
#define UART_2_TX_PIN_SOURCE    GPIO_PinSource2
#define UART_2_RX_PIN           GPIO_Pin_3
#define UART_2_RX_PIN_SOURCE    GPIO_PinSource3

static bool uart_2_received = false;
static RingBuffer *uart_2_rcv_buffer = 0;
static RingBuffer *uart_2_tx_buffer = 0;

static void config_hardware_uart_2(void);
static void config_driver_uart_2(uint32_t baudrate);

void uart_2_init(uint32_t baudrate)
{
    config_hardware_uart_2();
    config_driver_uart_2(baudrate);
}

bool uart_2_is_interupt(void)
{
    return uart_2_received;
}

void uart_2_clear_interupt(void)
{
    uart_2_received = false;
}

static void config_hardware_uart_2(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_PinAFConfig(UART_2_PORT, UART_2_TX_PIN_SOURCE, GPIO_AF_USART2);
    GPIO_PinAFConfig(UART_2_PORT, UART_2_RX_PIN_SOURCE, GPIO_AF_USART2);

    GPIO_InitStructure.GPIO_Pin = UART_2_TX_PIN | UART_2_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(UART_2_PORT, &GPIO_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void config_driver_uart_2(uint32_t baudrate)
{
     if (uart_2_rcv_buffer == 0) {
        uart_2_rcv_buffer = ring_buffer_create(512);
    }

    if (uart_2_tx_buffer == 0) {
        uart_2_tx_buffer = ring_buffer_create(512);
    }

    ring_buffer_empty(uart_2_rcv_buffer);
    ring_buffer_empty(uart_2_tx_buffer);

    USART_Cmd(USART2, DISABLE);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);
}

int uart_2_get(uint8_t *data)
{
    int length = ring_buffer_queue_length(uart_2_rcv_buffer);

    if (length > 0) {
        ring_buffer_get(uart_2_rcv_buffer, (char *)data, length);
        ring_buffer_empty(uart_2_rcv_buffer);
    }

    return length;
}

void uart_2_send(uint8_t *data, uint16_t length)
{
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

    ring_buffer_put(uart_2_tx_buffer, (char *)data, length);

    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

//UART 2 interupt handler
void USART2_IRQHandler(void)
{
    //Rx data interupt
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        // Received new data
        char c = USART_ReceiveData(USART2);
        ring_buffer_put_1(uart_2_rcv_buffer, c);
        uart_2_received = true;
    }

    //Tx data interupt
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
        // Txe Empty
        char c;

        if (ring_buffer_get_1(uart_2_tx_buffer, &c)) {
            USART_SendData(USART2, c);
        } else {
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }
}
