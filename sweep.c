/*
 * sweep.c
 *
 *  Created on: Nov 12, 2017
 *      Author: shanders
 */

#include "WiFi.h"
#include "UART.h"
#include "servo.h"
#include "ping.h"
#include "lcd.h"
#include "timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "ir.h"
#include <math.h>


char objectLoc[91];
char objectDist[91];
char smallObjectAngle[91];
char smallObjectLoc[91];
int shortObject = 0;
int shortAngle = 0;
int smallWidth = 0;

int sweep(void)
{
    //lcd_init();
    //char password[] = "Cpre_288";
    //WiFi_start(password);
    ir_init();
    ping_init();
    servo_init();
    timer1_init();
    UART_Init();
    char irDist[91];
    char pingDist[91];
    int angle = 0;
    //int indexCount = 0;
    //int positionIndex = 0;
    //int smallIndex = 180;
    //int firstFlag = 0;
    //int secondFlag = 0;

    while (1)
    {
        int i = 0;
        int k = 0;
        int j = 0;
        int m = 0;
        int n = 0;
        int p = 0;
        int degree = 0;
        int startPos = 0;
        int lastPos = 0;
       // int startShortPos = 0;
       // int lastShortPos = 0;
        int indexSize = 0;
       // int objectLength [91];
        int indexCount = 0;
        int positionIndex = 0;
       // int positionShort = 0;
        int smallIndex = 180;
        int firstFlag = 0;
       // int shortFlag = 0;
        int smallIR = 0;
        int smallPing = 0;
        int smallAngle = 0;
        int smallObject = 0;
        int startPing = 0;
        int lastPing = 0;
        int objectSize = 0;
        int smallCount = 0;
        int count = 0;


        //int firstLength = 0;
        //int secondLength = 0;


        timer_waitMillis(100);
        while (angle < 180)
        {
            servo_move(degree);
            angle = angle + degree;
            int irVal = 0;
            int sum = 0;
            for (i = 0; i < 16; ++i)
            {
                //Will give us our quantization value in channel 0
                irVal = ir_read(0);
                sum += irVal;
            }
            int irAvg = (sum / 16);
            //An easier way for us to write out the power in our function
            double power = pow(irAvg, -1.1403);
            //Our power function to calculate centimeters
            double irCenti = 82883.05518 * power;
            irDist[k] = (int) irCenti;

            //Get ping distance at current angle
            int delta = ping_read();
            int pingCenti = timetodist(delta);
            pingDist[j] = pingCenti;

            //checks to see if the short object is detected
            /*if((pingCenti>=30)&&(pingCenti<=70)&&(shortFlag == 0)){
                startShortPos = angle;
                shortFlag++;

            }
            if((pingCenti<30)||(pingCenti>70)&&(shortFlag == 1)){
                lastShortPos = angle;
                shortFlag = 0;
                shortObject = pingCenti;
                shortAngle = (lastPos + startPos)/2;

            }*/

            //Once an object is detected, records the first edge of object
            if((irCenti >= 5) && (irCenti <= 70) && (pingCenti <= 100) && (firstFlag == 0)){
             startPos = angle;
             startPing = pingCenti;
             //firstLength = pingDist[j];
             firstFlag++;
            }
            //Will continue to read until it is out of range and records the last edge of object
            if((irCenti < 5)||(irCenti > 70) && (pingCenti > 100) && (firstFlag == 1)){
                 lastPos = angle;
                 lastPing = pingCenti;
                 //secondLength = pingDist[j];
                 firstFlag = 0; //resets flag
                 indexCount++; //counts object
                 indexSize = lastPos - startPos; //Size of current index
                 positionIndex = (lastPos + startPos)/2; //Angle of current index
                 //objectLength[n] = sqrt(pow(firstLength,2)+ pow(secondLength,2) - 2*firstLength*secondLength*cos(indexSize));
                 objectLoc[m] = positionIndex; //Stores current location into an array
                 objectDist[m] =  pingDist[k]; //Stores current ping value to current object

                 //object size calc
                 objectSize = startPing*sin(indexSize/2) + lastPing*sin(indexSize/2);


                 //Check if the object size is smaller than the smallest object
                 if(objectSize < 16){
                         smallObjectAngle[p] = positionIndex;//Angle of smallest object
                         smallObjectLoc[p] = pingDist[k]; //Current locatation of small object
                         //smallObject = indexCount; //Current count is now the smallest index
                         smallCount++;
                         ++p;

                 }
                 m++;
                 n++;
            }
            //Prints off the LCD, then transmit message to putty
            char message[100];
            sprintf(message, "%d        %d         %d\n\r", angle, irDist[k],
                    pingDist[j]);
            puts(message);
            UART_TransmitMessage(message);
            lcd_printf("Degree:%d\nIR:%d\nPing:%d", angle, irDist[k],
                       pingDist[j]);
            //Increase all increments for the arrays and change degree to 2
            ++k;
            ++j;
            degree = 2;
            timer_waitMillis(100);


        }
        /*//Print total count of objects
        lcd_printf("Number of objects: %d",indexCount);
        timer_waitMillis(1000);
        //Print index size of smallest object
        lcd_printf("Size of object: %d",smallIndex);
        timer_waitMillis(1000);
        lcd_printf("Width of object: %d",smallWidth);
        timer_waitMillis(1000);*/
        //int sweepVal = -(180-objectLoc[secondFlag]+5);
        int sweepVal = -(180-smallAngle+5);
        //Once the sweep is completed, the servo will move to the smallest object on the field
        servo_move(sweepVal);
        //Prints Index number, degree, IR and Ping distance, then stops
        //lcd_printf("Index:%d\nDegree:%d\nIR:%d\nPing:%d",secondFlag,objectLoc[secondFlag],smallIR,smallPing);
        lcd_printf("Index:%d\nDegree:%d\nIR:%d\nPing:%d",smallObject,smallAngle,smallIR,smallPing);
        break;
    }
        return 1;
}


int distResults(int value){
    return objectDist[value];
}

int posResults(int value){
    return objectLoc[value];
}
int smallPosResult(int value){
    return smallObjectLoc[value];
}
int smallAngleResult(int value){
    return smallObjectAngle[value];
}
/*int shortWid(){
    return smallWidth;
}*/
