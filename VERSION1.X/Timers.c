/*
 * File:   Timers.c
 * Author: shanz
 *
 * Created on December 2, 2023, 12:00 PM
 */


#include "xc.h"

#include "Timers.h"

 //Take input of time delay in ms, and set PR2 value to count to specified delay
void delay_ms(uint16_t time_ms){
    PR2 = time_ms*2; //MATH FOR PR2 VALUE: time_ms(desired delay) = PR2*8(prescale)*2*1000/fclk(our 32kHz clock) -> PR2 = times_ms*32000/8*2*1000 -> PR2 = time_ms*(32000/16000) -> PR2 = time_ms*2
    TMR2 = 0;
    T2CONbits.TON = 1;

    
}

void delay_msT2(uint16_t time_ms){


    
}

void delay_msT3(uint16_t time_ms){
    

    
}




void clockSetup(){
    

    T1CONbits.TCKPS=3;
    T1CONbits.TCS=0; 
    T1CONbits.TSIDL=0;
    IPC0bits.T1IP=4;
    IFS0bits.T1IF=0;
    IEC0bits.T1IE=1;
    
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 3; // set pre-scalar = 8
    T2CONbits.TCS = 0; // use internal clock
    T2CONbits.TSIDL = 0; // operate in idle mode 
    IPC1bits.T2IP = 2; // priority config to 6 (1 priority under CN interrupt)
    IFS0bits.T2IF = 0; // clear interrupt flag
    IEC0bits.T2IE = 1; // enable timer interrupt
    
    
    T3CONbits.TCKPS=3;
    T3CONbits.TCS=0; 
    T3CONbits.TSIDL=0;
    IPC2bits.T3IP=4;
    IFS0bits.T3IF=0;
    IEC0bits.T3IE=1;
}   


//T2 Interrupt causes light to switch 1->0 or 0->1
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void){
    IFS0bits.T2IF=0; 
    }



//T2 Interrupt causes light to switch 1->0 or 0->1
void __attribute__((interrupt, no_auto_psv))_T1Interrupt(void){
       IFS0bits.T1IF=0;
    }



//T2 Interrupt causes light to switch 1->0 or 0->1
void __attribute__((interrupt, no_auto_psv))_T3Interrupt(void){
    IFS0bits.T3IF=0;

    }