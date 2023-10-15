/* 
 * File:   main.c
 * Author: Alexa
 *
 * Created on October 2, 2023, 9:47 AM
 */

#include "xc.h"
#include <p24F16KA101.h>

#define time1 10000 //delay for ~0.5 seconds
#define time2 40000 //delay for ~1.0 seconds
#define time3 80000 //delay for ~2.0 seconds

int main(void) {
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
    //Setting counter variable as unsigned long for large counts
    unsigned long count = 0;
    while(1){
        count = 0;
        //RA2 (PB1) (only) pressed == 0.5s delay blinking
        while(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1)
        {
            //LED blinks on/off at 0.5s interval
            if(count > time1 && count < time1*2)
            {
                LATBbits.LATB8 = 1;
            }
            if(count > time1*2){ //At 2*count, resets the counter to cycle through the timed loop
                count = 0;
                LATBbits.LATB8 = 0;
            }
            count++;
        }
        count = 0; //Reset count every time state changes
       
        //RA4 (PB2) (only) pressed == 1.0s delay blinking
        while(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA2 == 1)
         {
            //LED blinks on/off at 0.5s interval
            if(count > 40000 && count < 40000*2)
            {
                LATBbits.LATB8 = 1;
            }
            if(count > 40000*2){ //At 2*count, resets the counter to cycle through the timed loop
                count = 0;
                LATBbits.LATB8 = 0;
            }
            count++;
        }
        count = 0;

        //RB4 (PB3) (only) pressed == 2.0s delay blinking
        while(PORTBbits.RB4 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1){
            //LEB blinks on/off at 2s interval
             if(count > time3 && count < time3*2)
            {
                LATBbits.LATB8 = 1;
            }
            if(count > time3*2){ //At 2*count, resets the counter to cycle through the timed loop
                count = 0;
                LATBbits.LATB8 = 0;
            }
            count++;
        }
        count = 0;
     
        //Next 3 while loops catch all cases of multiple PBs pressed == light ON (no blink))
        while(PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0)//RA2&&RA4
        {
            LATBbits.LATB8 = 1; //LED on pin RB8 in ON state.
        }
        
        while(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 0)//RA2&&RB4
        {
            LATBbits.LATB8 = 1;//LED on pin RB8 in ON state.
        }
    
        while(PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)//RA4&&RB4
        {
            LATBbits.LATB8 = 1; //LED on pin RB8 in ON state.
        }
        
        //All other cases == no inputs == light OFF. If no PBs are pressed, the loop will continuously run to set the light OFF.
        LATBbits.LATB8 = 0; // LED on pin RB8 in OFF state.
    }
    return 0;
}