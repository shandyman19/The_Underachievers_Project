/*
 * servo.c
 *
 *  Created on: Oct 17, 2017
 *      Author: shanders
 */

#include "servo.h"
#include "lcd.h"
#include "timer.h"
#include <stdint.h>
//#include <stdbool.h>

unsigned pulse_period = 320000;
volatile unsigned pulse_width = 8070;
//Gives us the value of pulse period - pulse width
volatile unsigned pulseValue = 8070;
volatile int button = 0; // Used to check to see if we are increasing or decreasing value
void servo_init(){
    //Turn on PORT B system clock
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //Enable clock to Port B
    SYSCTL_RCGCTIMER_R |= 0x02;
    //Make pin our output
    GPIO_PORTB_DIR_R |= 0x20;
    //Pin 5 is digital
    GPIO_PORTB_DEN_R |= 0x20;
    //Enable port B pin 3 to work alternate functions
    GPIO_PORTB_AFSEL_R |= 0x20;
    //Activate Pin 3 alternate function
    GPIO_PORTB_PCTL_R|= 0x00700000;
    button = 0;


}

void timer1_init(){
        //Enable clock to Port B
        SYSCTL_RCGCTIMER_R |= 0x02;
        TIMER1_CTL_R &= ~0x100;//disable timer to config
        TIMER1_TBMR_R |= 0x02;  //periodic enable
        TIMER1_TBMR_R |= 0x08; // PWN enable
        TIMER1_CFG_R |= 0x04;//set size of timer to 16
        TIMER1_TBILR_R = (pulse_period & 0xFFFF); //lower 16 bits of the interval
        TIMER1_TBPR_R = (pulse_period >> 16); //set the upper 8 bits of the interval
        pulse_width = 8070; //Starting position at 0 degrees
        TIMER1_TBMATCHR_R = ((pulse_period-pulse_width) & 0xFFFF); //lower 16 bits of the interval
        TIMER1_TBPMR_R = ((pulse_period-pulse_width) >> 16); //set the upper 8 bits of the interval
        TIMER1_CTL_R |= 0x100; //enable timer
}

void servo_move(signed degree){
    //add our pulse_width with our scalar to our new pulseValue
    pulseValue = (degree * 160) + pulse_width;
    TIMER1_TBMATCHR_R = ((pulse_period-pulseValue) & 0xFFFF); //lower 16 bits of the interval
    TIMER1_TBPMR_R = ((pulse_period-pulseValue) >> 16); //set the upper 8 bits of the interval
    //Current pulseValue is new pulse_width
    pulse_width = pulseValue;
    timer_waitMillis(100);
}

unsigned getVal(){
    //Returns value at current position
    return pulseValue;
}





