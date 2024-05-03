/*
 * File:   IOinit.c
 * Author: shanz
 *
 * Created on December 2, 2023, 11:36 AM
 */



#include "xc.h"
//include <p24F16KA101.h>
#include "IOinit.h"


//extern uint8_t CNflag;
 void IOinit(){
     
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
    
    //Setting input pins to allow for CN interrupts
    CNEN1bits.CN0IE = 1; // Enables CN interrupt on CN0
    CNEN1bits.CN1IE = 1; // Enables CN interrupt on CN1
    CNEN2bits.CN30IE = 1; // Enables CN interrupt on CN30
    
    // CN Interrupt Settings
    IFS1bits.CNIF = 0; // Clear CN interrupt flag
    IPC4bits.CNIP = 7; // set priority to highest (1 lowest, 0 off)
    IEC1bits.CNIE = 1; // Enable CN interrupts
    
    return;
}
 
 
 
 void IOcheck(){ 

     
     
     
} 

//CN interrupt flips global flag variable that main function can read to check which input was given.
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{ 
    //CNflag = 1; // Global user defined flag - use only if needed
    IFS1bits.CNIF = 0;  // Clear IF flag
}
