/*
 * servo.h
 *
 *  Created on: Oct 17, 2017
 *      Author: shanders
 */

#ifndef SERVO_H_
#define SERVO_H_

void servo_init(void);

void servo_move(signed degree);

void timer1_init(void);

unsigned getVal();


#endif /* SERVO_H_ */
