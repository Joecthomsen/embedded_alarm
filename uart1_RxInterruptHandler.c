/*
 * File:   uart1_RxInterruptHandler.c
 * Author: Joect
 *
 * Created on April 6, 2023, 11:14 AM
 */


#include <xc.h>

#include "uart1_RxInterruptHandler.h"
#include "mcc_generated_files/uart1.h"
#include <stdio.h>

void uart1_RxInterruptHandler() {
    
    int k = 0;
    printf("%c", UART1_Read());
    
    return;
}
