/*
 * main.c
 */

/*
 * Logs update:
 * 11/12/17: Made skeleton for cliff sensors and putty commands
 * 11/13/17: Manuel movement ready, and cliff detect not working properly
 * (Fine with hole, not with white tape)
 */

#include "WiFi.h"
#include "movement.h"
#include "open_interface.h"
#include "UART.h"
#include "sweep.h"
#include "servo.h"
#include "ping.h"
#include "lcd.h"
#include "timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "ir.h"
#include <math.h>

int main(void) {
	  //First we initalize our functions
      lcd_init();
      //char password[] = "Cpre_288";
      //WiFi_start(password);
      oi_t *sensor_data = oi_alloc();
      oi_init(sensor_data);
      //Use to tell code when our sweep is done
      int doneSweep;
      char objectLoc[91];
      char objectDist[91];
      while(1){
          int i = 0;

          //First, sweeps the area for objects
          doneSweep = sweep();
          //Reads through each object's location and distance from bot and prints onto putty for us to see
          while(doneSweep == 1){
	       for(i = 0; i < 91; i++){
	           objectLoc[i] = posResults(i); //Gets object's location at that specific value
	           objectDist[i] = distResults(i); //Gets object's distance at that specific value
	           if(objectLoc[i] == 0 && objectDist[i] == 0){//Once all objects are printed out, break out of loop
	               break;
	           }
	           char message2[100];
	           sprintf(message2, "Object %d: Angle %d  Distance  %d\n\r" ,i+1, objectLoc[i], objectDist[i]);
	           puts(message2);
	           UART_TransmitMessage(message2);
	       }
	       //Putty will tells us what to do next and use one of the commands belong to excute
	       char message[100];
	       sprintf(message, "What is your command? \n\r");
	       puts(message);
	       UART_TransmitMessage(message);
	       char command = UART_Receive();
	       timer_waitMillis(500);

	       //Moves bot, if cliff is detected, stop and turn
	       if(command == 'w'){
	           move_forward(sensor_data, 400);
	           if((sensor_data->cliffLeft) || ((sensor_data->cliffLeftSignal) > 2790)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	               turn_clockwise(sensor_data, -90);
	           }
	           else if((sensor_data->cliffFrontLeft) || ((sensor_data->cliffFrontLeftSignal) > 2780)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	               turn_clockwise(sensor_data, -90);
	           }
	           else if((sensor_data->cliffFrontRight) || ((sensor_data->cliffFrontRightSignal) > 2750)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	               turn_counterclockwise(sensor_data, 90);
	           }
	           else if((sensor_data->cliffRight) || ((sensor_data->cliffRightSignal) > 2740)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	               turn_counterclockwise(sensor_data, 90);
	           }
	           else if(sensor_data->bumpLeft){
	               oi_setWheels(0,0);
	               move_backwards(sensor_data, -100);
	               turn_clockwise(sensor_data, -90);
	           }
	           else if(sensor_data->bumpRight){
	               oi_setWheels(0,0);
	               move_backwards(sensor_data, -100);
	               turn_counterclockwise(sensor_data, 90);
	           }
	       }
	       if(command == 's'){
	           move_backwards(sensor_data,-200);
	           }
	       if(command == 'a'){
	           turn_counterclockwise(sensor_data, 92);
	           }
	       if(command == 'd'){
	           turn_clockwise(sensor_data, -89);
	           }

	       }
	       //Reset doneSweep to 0 and sweeps again
	       doneSweep = 0;
	   }
	}
	return 0;
}
