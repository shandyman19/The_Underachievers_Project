/*
 * main.c
 */

/*
 * Logs update:
 * 11/12/17: Made skeleton for cliff sensors and putty commands
 * 11/13/17: Manuel movement ready, and cliff detect not working properly
 * (Fine with hole, not with white tape)
 * 11/28/17: Attempt to work on getting to endzone, finished with music and flashing LED.
 *              There might be a way to find with using both ping and ir that are in agreement
 */

#include "WiFi.h"
#include "movement.h"
#include "open_interface.h"
#include "timer.h"
#include "music.h"
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
	  //First we initialize our functions
      lcd_init();
      //char password[] = "Cpre_288";
      //WiFi_start(password);
      oi_t *sensor_data = oi_alloc();
      oi_init(sensor_data);
      //Use to tell code when our sweep is done
      int doneSweep;
      char objectLoc1[91];
      char objectDist1[91];
      char smallObjectAngle1[91];
      char smallObjectLoc1[91];
      int count = 0;
      while(1){
          if(count == 0){
          int i = 0;
          //First, sweeps the area for objects
          doneSweep = sweep();
          //Reads through each object's location and distance from bot and prints onto putty for us to see
          while(doneSweep == 1){
	       for(i = 0; i < 91; i++){
	           objectLoc1[i] = posResults(i); //Gets object's location at that specific value
	           objectDist1[i] = distResults(i); //Gets object's distance at that specific value
	           if(objectLoc1[i] == 0 && objectDist1[i] == 0){//Once all objects are printed out, break out of loop
	               break;
	           }
	           char message2[100];
	           sprintf(message2, "Object %d: Angle %d  Distance  %d\n\r" ,i+1, objectLoc1[i], objectDist1[i]);
	           puts(message2);
	           UART_TransmitMessage(message2);
	       }
	       //Reads through each skinny object's location and distance from bot and prints onto putty for us to see
	       for(i = 0; i < 91; ++i){
	           smallObjectAngle1[i] = smallAngleResult(i);
	           smallObjectLoc1[i] = smallPosResult(i);
	           if(smallObjectAngle1[i] == 0 && smallObjectLoc1[i] == 0){
	               break;
	           }
	           char message3[100];
	           sprintf(message3, "Small Object %d: Angle %d  Distance  %d\n\r" ,i+1, smallObjectAngle1[i], smallObjectLoc1[i]);
	           puts(message3);
	           UART_TransmitMessage(message3);
	       }
	       doneSweep = 0;
	       count = 1;
          }
          }

	       //Putty will tells us what to do next and use one of the commands belong to excute
	       int pingSome = getPing();
           char message[100];
	       sprintf(message, "What is your command? \n\r");
	       puts(message);
	       UART_TransmitMessage(message);
	       char command = UART_Receive();
	       timer_waitMillis(500);

	       //Moves bot, if cliff is detected, stop and turn
	       if(command == 'w'){
	           move_forward(sensor_data, 100);
	           if(sensor_data->cliffLeft){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }
	           else if((sensor_data->cliffLeftSignal) > 2790){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff Tape on left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if(sensor_data->cliffFrontLeft){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if((sensor_data->cliffFrontLeftSignal) > 2780){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff Tape on front left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if(sensor_data->cliffFrontRight){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if((sensor_data->cliffFrontRightSignal) > 2750){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff Tape on front right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if(sensor_data->cliffLeft){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);
	           }
	           else if((sensor_data->cliffLeftSignal) > 2740){
	           oi_setWheels(0,0);
	           char cliffMessage[100];
	           sprintf(cliffMessage, "Cliff Tape on right side! \n\r");
	           puts(cliffMessage);
	           UART_TransmitMessage(cliffMessage);
	           move_backwards(sensor_data, -100);
	           }
	           /*else if((sensor_data->cliffFrontLeft) || ((sensor_data->cliffFrontLeftSignal) > 2780)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }
	           else if((sensor_data->cliffFrontRight) || ((sensor_data->cliffFrontRightSignal) > 2750)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on front right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }
	           else if((sensor_data->cliffRight) || ((sensor_data->cliffRightSignal) > 2740)){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Cliff on right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }*/
	           else if(sensor_data->bumpLeft){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Object on left side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }
	           else if(sensor_data->bumpRight){
	               oi_setWheels(0,0);
	               char cliffMessage[100];
	               sprintf(cliffMessage, "Object on right side! \n\r");
	               puts(cliffMessage);
	               UART_TransmitMessage(cliffMessage);
	               move_backwards(sensor_data, -100);

	           }
	       }
	       //Backwards
	       if(command == 's'){
	           move_backwards(sensor_data,-100);
	           }
	       //Left 90 degrees
	       if(command == 'a'){
	           turn_counterclockwise(sensor_data, 92);
	           }
	       //Right 90 degrees
	       if(command == 'd'){
	           turn_clockwise(sensor_data, -89);
	           }
	       //Left 20 degrees
	       if(command == 'q'){
	           turn_counterclockwise(sensor_data, 20);
	       }
	       //right 20 degrees
	       if(command == 'e'){
	           turn_clockwise(sensor_data, -20);
	       }
	       //Sweeps again
	       if(command =='r'){
	           int i = 0;
	           doneSweep = sweep();
	           //Reads through each object's location and distance from bot and prints onto putty for us to see
	           while(doneSweep == 1){
	              for(i = 0; i < 91; i++){
	                 objectLoc1[i] = posResults(i); //Gets object's location at that specific value
	                 objectDist1[i] = distResults(i); //Gets object's distance at that specific value
	                     if(objectLoc1[i] == 0 && objectDist1[i] == 0){//Once all objects are printed out, break out of loop
	                         break;
	                         }
	                 char message2[100];
	                 sprintf(message2, "Object %d: Angle %d  Distance  %d\n\r" ,i+1, objectLoc1[i], objectDist1[i]);
	                 puts(message2);
	                 UART_TransmitMessage(message2);
	                 }
	              for(i = 0; i < 91; ++i){
	                 smallObjectAngle1[i] = smallAngleResult(i);
	                 smallObjectLoc1[i] = smallPosResult(i);
	                     if(smallObjectAngle1[i] == 0 && smallObjectLoc1[i] == 0){
	                        break;
	                        }
	              char message3[100];
	              sprintf(message3, "Small Object %d: Angle %d  Distance  %d\n\r" ,i+1, smallObjectAngle1[i], smallObjectLoc1[i]);
	              puts(message3);
	              UART_TransmitMessage(message3);
	          }
	          doneSweep = 0;
	          count = 1;
	         }

	       }
	       //Once we are in the zone, plays music and flash leds and end bot movement and sweep
	       if(command == 'l'){
	           load_songs();
	           oi_setLeds(1, 1, 0, 255);
	           timer_waitMillis(100);
	           oi_setLeds(0, 0, 0, 0);
	           timer_waitMillis(100);
	           oi_setLeds(1, 1, 0, 255);
	           timer_waitMillis(100);
	           oi_setLeds(0, 0, 0, 0);
	           timer_waitMillis(100);
	           oi_setLeds(1, 1, 0, 255);
	           timer_waitMillis(100);
	           oi_setLeds(0, 0, 0, 0);
	           timer_waitMillis(100);
	           oi_setLeds(1, 1, 0, 255);
	           count = 1;


	       }
	  }



	return 0;
}
