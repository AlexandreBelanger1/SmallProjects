/* 
 * File:   main.c
 * Author: Alexandre
 *
 * Created on December 2, 2023, 12:18 PM
 */

#include <stdio.h>
#include <stdlib.h>
// PIC24F16KA101 Configuration Bit Settings

/*
 * I recommend adding the FWDTEN setting to OFF pragma, as this stops your microcontroller periodically resetting itself when nothing happens
 */
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))
// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON          // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
/*
 * Adding the FCKSM = CSECMD enables clock switching
 */
#pragma config FCKSM = CSECMD           // Clock Switching and Monitor Selection (Clock switching is enabled, Fail-Safe Clock Monitor is disabled)
/*
 * This pragma enables using the stepping debugger/setting breakpoints on the microcontroller
 */
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

#include <xc.h>
#include "clkChange.h"
#include "Uart.h"
#include "ADC.h"
#include "Timers.h"
#include "main.h"
#include "IOs.h"

#define BUF_SIZE 10
extern uint8_t RXFlag;
extern uint8_t received_char;

uint16_t timerFlag = 0;

#define S0 0 // default state, wait for first user input, change upon hitting enter
#define S1 1 // 1st char correct, wait for second char and blink LED
#define S2 2 // 2nd char correct, wait for third char and blink LED
#define S3 3 // Password correct, activate potentiometer, select value with it and enter by pressing PB2, LED blinks FASTER
#define S4 4 // Potentiometer correct, press pb1 for re-lock, or PB2 to enter new password, or press PB3 to enter special "unlocked" state
#define S5 5 // Special unlocked state, do something creative involving clk switching
#define S6 6 // State to enter a new password
#define S7 7 // Error state! check previous state and give an appropriate error message before returning to S0

#define CLEAR_DISP Disp2String("                                                \r") //clears the previous terminal line
#define LED_ON LATBbits.LATB8 = 1
#define LED_OFF LATBbits.LATB8 = 0
#define PB1 PORTAbits.RA2
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA4

extern uint8_t RXFlag;
extern uint8_t received_char;
uint16_t ADCoutput;
int PB1state = 0;
int PB2state = 0;
int PB3state = 0;
int buttonState=0;
int CNflag=0;
int state_check;
int state = 0;
int count = 0;
char guess;
char pass[] = {'a','b','c'};
int potentiometerPass = 5;
/*
 * 
 */
int main() {
    //Initializing 
    InitUART2();
    clockSetup();
    delay_ms(50);
    ADC_setup();
    IOinit();
    
    while(1)
    {
        
        
        
        //State machine
        if(state == S0) //Input first char of password
        {
            if(RXFlag)
            {
                guess = RecvUartChar(); // get keyboard input
                if(guess == pass[0]) // if match -> next state
                {
                    state = S1;
                }
                else // if no match -> error state
                {
                state = S7;
            }
            
            }   
        }
        else if(state == S1) //Input 2nd char of password
        {
            if(RXFlag)
            {
                guess = RecvUartChar(); // get keyboard input
                if(guess == pass[1]) // if match -> next state
                {
                    state = S1;
                }
                else // if no match -> error state
                {
                state = S7;
            }
            
            }
        }
        else if(state == S2) //Input 3rd char of password
        {
            if(RXFlag)
            {
                guess = RecvUartChar(); // get keyboard input
                if(guess == pass[2]) // if match -> next state
                {
                    state = S1;
                }
                else // if no match -> error state
                {
                state = S7;
            }
            
            }
        }
        else if(state == S3) //Input ADC value of password
        {
            Disp2String("\r");
            ADCoutput = do_ADC(); //Enables ADC sampling and saves sampled value
            print_ADC(ADCoutput); //Generate bar graph   
        }
        else if(state == S4) // Choose between password reset, program restart, or special state
        {
            
        }
        else if(state == S5) //Special state unlocked
        {
            do_something();
        }
        else if(state == S6) //Input new password
        {
            
        }
        else if(state == S7) //Error . Pressing PB2 restarts to initial state.
        {
            Disp2String("Error! Please press PB2 to restart.");
        //Button checking
            while(state == S7){
        if (CNflag == 1) // Check for for CN Interrupt state of 1: a button is pressed
        {
            CNflag = 0;
            buttonState = debounce(); // We know button(s) pressed, find which one(s) and what to do about it.
         }
        else if(CNflag == 2){ //All buttons are now released, 
          CNflag = 0;
          if(buttonState == 2)
          {
              state = S0;
          }
        }
        
        }
    }
    }
    return (EXIT_SUCCESS);
}

 //CN interrupt flips global flag variable that main function can read to check which input was given.
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{ 
    
    //CN ISR checks previous button states against current inputs to either set the button state, or once  buttons are released, enter the corresponding state as instructed by button state
    if(PB1state == 1 && PB1 == 0) //Flag CN on Low->High only
    {
        CNflag = 1;
    }
    else if(PB2state == 1 && PB2 == 0)
    {
        CNflag = 1;
    }
    else if(PB3state == 1 && PB3 == 0)
    {
        CNflag = 1;
    }
    if(PB1 == 1 && PB2 == 1 && PB3 == 1) //Flag once all inputs released
    {
        CNflag = 2; //Main interprets this as the go-ahead to enter the state as instructed by button state
    }
    IFS1bits.CNIF = 0;
    PB1state = PB1; //Update previous button state to current state
    PB2state = PB2;
    PB3state = PB3;
    
}

int debounce(void) {
    int state = IOcheck(); //Get button info
    count =0;
    while(count<500) //Delay for a bit
    {
        count++;
    }
    state_check = IOcheck(); //Get  button info again
    if(state == state_check) { //Return button state info
        return state;
    }
    return 0;
}
