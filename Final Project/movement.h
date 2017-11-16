/*
 * movement.h
 *
 *  Created on: Sep 5, 2017
 *      Author: shanders
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"


int move_forward(oi_t*sensor, int millimeters);

void turn_clockwise(oi_t*sensor, int16_t degree);

void turn_counterclockwise(oi_t*sensor, int16_t degree);

int move_backwards(oi_t*sensor, int millimeters);

#endif /* MOVEMENT_H_ */
