/* 
 * File:   uart.h
 * Author: andi
 *
 * Created on 20. November 2013, 14:17
 */

#ifndef UART_H
#define	UART_H

void initUART();
void uart_putc(BYTE);
void uart_print(BYTE *);
BYTE uart_rx_buff[];
BYTE uart_rx_count;
#endif	/* UART_H */

