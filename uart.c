#include <plib.h>
#include "./uart.h"

BYTE uart_rx_buff[16];
BYTE uart_rx_count=0;

void initUART()

{
    // PPS map UART1 pins
    RPB3R=0x01;  // PPS MAP TX to RPB3
    U1RXR=0x04;  // PPS MAP RX to RPB2

    // Setup UART1
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART1, UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, 48000000, 115200);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_ENABLE | UART_PERIPHERAL | UART_RX | UART_TX));
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);
}

void uart_putc(BYTE ch)
{
    while(!UARTTransmitterIsReady(UART1));
    UARTSendDataByte(UART1, ch);
}

void uart_print(BYTE *string)
{
    while(!string)
    {
        while(!UARTTransmitterIsReady(UART1));
        UARTSendDataByte(UART1, *string);
        string++;
        while(!UARTTransmissionHasCompleted(UART1));
    }
}

void __ISR(_UART1_VECTOR, IPL2SOFT) IntUart1Handler(void)
{
     if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
     {
         INTClearFlag(INT_SOURCE_UART_RX(UART1));
         if (uart_rx_count<sizeof(uart_rx_buff)-1)
             uart_rx_count++;
         else
             uart_rx_count=0; // overflow
         
         uart_rx_buff[uart_rx_count]=UARTGetDataByte(UART1);
     }

    if(INTGetFlag(INT_SOURCE_UART_TX(UART1)))
   {
       INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}