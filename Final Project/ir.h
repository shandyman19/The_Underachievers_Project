#include "timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
/*
 * ir.h
 *
 *  Created on: Oct 3, 2017
 *      Author: shanders
 */

#ifndef IR_H_
#define IR_H_

void ir_init();

unsigned ir_read(char channel);

int ir_Avg(int sum);



#endif /* IR_H_ */
