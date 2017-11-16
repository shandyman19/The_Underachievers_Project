/*
 * ping.c
 *
 *  Created on: Oct 10, 2017
 *      Author: shanders
 */
#include "ping.h"
#include "timer.h"
#include "lcd.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

volatile enum {LOW, HIGH, DONE, READY, } sonar_state;
volatile unsigned rise_time; // start time of the return pulse
volatile unsigned fall_time; // end time of the return pulse
volatile int flag = 0;

void ping_init(void){
    //Enable Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //Enable clock to Port B
    SYSCTL_RCGCTIMER_R |= 0x08;
    //Disable Timer3 on port B
    TIMER3_CTL_R &= ~0x100;
    //Enable both edges
    TIMER3_CTL_R |= 0xC00;
    //Enable a 16bit timer to Timer 3
    TIMER3_CFG_R |= 0x04;
    //Enable capture mode
    TIMER3_TBMR_R|= 0x03;
    //Enable Edge time mode
    TIMER3_TBMR_R|= 0x04;
    //Count up
    TIMER3_TBMR_R|= 0x10;

    //Set upper bound of 16bits
    TIMER3_TBILR_R|=0xFFFE;
    //Enable Capture mode for interrupts
    TIMER3_IMR_R|= 0x400;
    TIMER3_ICR_R |= 0x400;
    //Initialize local interrupts
    NVIC_EN1_R = 0x00000010;
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable(); //intialize global interrupts
    //Enable Timer 3
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){
//Check for rising and falling edges
    if(flag == 0){
        rise_time = TIMER3_TBR_R;
        flag++;
        }
    else if(flag == 1){
        fall_time= TIMER3_TBR_R;
        flag++;
        }
    TIMER3_ICR_R |= 0x400;

}
void ping_send(){
    flag = 0;
    //Disable Timer3 on port B
    TIMER3_CTL_R &= ~0x100;
    //Make pin our output
    GPIO_PORTB_DIR_R |= 0x08;
    //Pin 3 is digital
    GPIO_PORTB_DEN_R |= 0x08;
    //Data for pin 3 is high
    GPIO_PORTB_DATA_R |= 0x08;
    //Disable port B pin 3 to work alternate functions
     GPIO_PORTB_AFSEL_R &= 0xF7;
    //Wait 5 microseconds
    timer_waitMicros(5);
    //Set Pin 3 to low
    GPIO_PORTB_DATA_R &= 0xF7;
    //Enable port B pin 3 to work alternate functions
    GPIO_PORTB_AFSEL_R |= 0x08;
    //Activate Pin 3 alternate function
    GPIO_PORTB_PCTL_R|= 0x00007000;
    //Pin 3 is input
    GPIO_PORTB_DIR_R &= 0xF7;
    //Enable Timer 3
    TIMER3_CTL_R |= 0x100;
}

int ping_read(){
    unsigned long timeDifference;
    ping_send();
    //waits for the handler to finish checking the edges before continuing this code
    while(flag < 2){

    }
    timeDifference = fall_time - rise_time;
    int overflow = getOverflow();
      if(overflow > 0){
      timeDifference = ((unsigned long)overflow<<24) + (fall_time-rise_time);
    }
    return timeDifference;

}

unsigned timetodist(unsigned time){
   //.0011 =
    return (time*.0011);
}

int getOverflow(){
    static int overflow = 0; // this has global lifetime, it saves its value.
    overflow+= (fall_time < rise_time);
    return overflow;
}

double disttoseconds(double dist){
    return dist/34.0;
}


