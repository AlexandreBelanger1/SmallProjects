/* 
 * File:   main.c
 * Author: Alexandre
 *
 * Created on October 15, 2023, 3:23 PM
 */

#include "xc.h"
#include <p24F16KA101.h>

/*
 * 
 */

void IOsetup(){
        //Setting inputs
    TRISAbits.TRISA2 = 1; //RA2 as input
    TRISAbits.TRISA4 = 1; //RA4 as input
    TRISBbits.TRISB4 = 1; //RB4 as input
    AD1PCFG = 0xFFFF; // Set all analog to digital io 
    //Setting outputs
    TRISBbits.TRISB8 = 0; //RB4 as input//RB8 as output
    
    //enable pull-up on buttons (pressing button will flip input from 1 to 0)
    CNPU2bits.CN30PUE = 1; //RA2
    CNPU1bits.CN1PUE = 1; //RB4
    CNPU1bits.CN0PUE = 1; //RA4
    return();
}

void TimerSetup(){
    
//T2CON config
T2CONbits.T32 = 0; // set timer 2 as 32 bit timer
T2CONbits.TCKPS = 3; // set timer 2 prescaler to 4th setting (256) (1,8,64,256)
T2CONbits.TCS = 0; //Use internal RC clock
T2CONbits.TSIDL = 0; // operate in idle mode (power saving)

//Timer 2 interrupt config
IPC1bits.T2IP = 2; //setting interrupt priority
IFS0bits.T2IF = 0; // set interrupt flag to 0
IEC0bits.T2IE = 1; //enables timer interrupt

PR2 = 8000; // When TMR2 == PR2, flag interrupt, reset TMR2 count
TMR2 = 0; //Increment +1 every prescaler*2 /Fclk, limit at 32 bits 4,294,967,295
T2CONbits.TON = 1; //turns timer on
return();
}

int main(){
    TimerSetup();
    IOsetup();
    while(1){
        
    }
    
    return(1);
}


//Timer 2 interrupt subroutine
void ___attribute___((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    TMR2flag = 1;
    LATBbits.LATB8 ^= 1;
}
