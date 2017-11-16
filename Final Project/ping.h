/*
 * ping.h
 *
 *  Created on: Oct 10, 2017
 *      Author: shanders
 */

#ifndef PING_H_
#define PING_H_

void ping_init(void);

void ping_send(void);

int ping_read(void);

unsigned timetodist(unsigned time);

int getOverflow();

void TIMER3B_Handler(void);

double disttoseconds(double dist);

#endif /* PING_H_ */
