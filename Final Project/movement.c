/*
 * movement.c
 *
 *  Created on: Sep 5, 2017
 *      Author: shanders
 */
#include "lcd.h"
#include "timer.h"
#include "open_interface.h"
#include "movement.h"



int move_forward(oi_t*sensor, int millimeters){
        int sum = 0;
        oi_setWheels(150, 150); // move forward; half speed
        while (sum < millimeters) {
            oi_update(sensor);
            sum += sensor->distance;
            if(sensor->bumpLeft){
                break;
            }
            else if(sensor ->bumpRight){
                break;
            }
            else if(sensor->bumpLeft && sensor->bumpRight){
                break;
            }
            //Detect hole or white tape
            else if((sensor->cliffLeft) || ((sensor->cliffLeftSignal) > 2790)){
                break;
            }
            else if((sensor->cliffRight) || ((sensor->cliffRightSignal) > 2740)){
                break;
            }
            else if((sensor->cliffFrontLeft) || ((sensor->cliffFrontLeftSignal) > 2780)){
                break;
            }
            else if((sensor->cliffFrontRight) || ((sensor->cliffFrontRightSignal) > 2750)){
                break;
            }


        }
        oi_setWheels(0, 0); //stop
        return sum;

        //oi_free(sensor);
}

void turn_counterclockwise(oi_t*sensor, int16_t degree){
            int16_t sum = 0;
            oi_setWheels(250, -250); // move forward; full speed
            while (sum < degree) {
                oi_update(sensor);
                sum += sensor->angle;
            }
            oi_setWheels(0, 0); // stop

            //oi_free(sensor);
}

void turn_clockwise(oi_t*sensor, int16_t degree){
               int16_t sum = 0;
               oi_setWheels(-250, 250); // move forward; full speed
               while (sum > degree) {
                   oi_update(sensor);
                   sum+= sensor->angle;
               }
               oi_setWheels(0, 0); // stop

               //oi_free(sensor);
}

int move_backwards(oi_t*sensor, int millimeters){
         int sum = 0;
        oi_setWheels(-150, -150); // move forward; half speed
        while (sum > millimeters) {
            oi_update(sensor);
            sum += sensor->distance;
        }
        oi_setWheels(0, 0);
        return sum;
}
