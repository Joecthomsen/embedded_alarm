 
#ifndef UART_HANDLER_H
#define	UART_HANDLER_H

#define UART_BUFFER_SIZE 1025

#include <xc.h> 

extern char uart_buffer[];
extern int uart_buffer_index;   

void uart_interrupt_handler();
void clearUartBuffer();

#endif	/* UART_HANDLER_H */

