#include "uart_2.h"

#include "stm32f4xx_usart.h"

#include "device_config.h"

#include "ring_buffer.h"

static bool uart_2_received = false;
static RingBuffer *uart_2_rcv_buffer = 0;
static RingBuffer *uart_2_tx_buffer = 0;

static void config_hardware_uart_2(void);
static void config_dma_1(void);
static void config_driver_uart_2(uint32_t baudrate);

void uart_2_init(uint32_t baudrate)
{
    config_hardware_uart_2();
    config_dma_1();
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
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;

    //Configure Tx DMA
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)data;

    DMA_Init(DMA1_Stream6, &DMA_InitStructure);

    //DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Stream6, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    while (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6)==RESET);

    USART_ClearFlag(USART2, USART_FLAG_TC);
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
}

static void config_hardware_uart_2(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART_2_IRQ_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void config_dma_1(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = DMA_1_IRQ_PRIORITY;
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

void DMA1_Stream6_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_Stream6, DMA_FLAG_TCIF6)) {
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_FLAG_TCIF6);
    }
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
