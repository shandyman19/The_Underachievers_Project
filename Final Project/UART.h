/*
 * UART.h
 *
 *  Created on: Sep 26, 2017
 *      Author: shanders
 */

#ifndef UART_H_
#define UART_H_

void UART_Init(void);

void UART_Transmit(char sdata);

void UART_TransmitMessage(char chr[]);

char UART_Receive(void);

void Wifi_stop();

void Wifi_start();



#endif /* UART_H_ */
