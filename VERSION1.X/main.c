/*
 * File:   main.c
 * Author: shanz
 *
 * Created on December 2, 2023, 11:27 AM
 */


#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
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
#include "uart.h"
#include "Timers.h"
#include "ADC.h"
#include "SpecialState.h"


#define LED_ON LATBbits.LATB8 = 1
#define LED_OFF LATBbits.LATB8 = 0
#define PB1 PORTAbits.RA2
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA4
extern uint8_t RXFlag;
extern uint8_t received_char;


#define S0 0 // default state, wait for first user input, change upon hitting enter
#define S1 1 // 1st char correct, wait for second char and blink LED
#define S2 2 // 2nd char correct, wait for third char and blink LED
#define S3 3 // Password correct, activate potentiometer, select value with it and enter by pressing PB2, LED blinks FASTER
#define S4 4 // Potentiometer correct, press pb1 for re-lock, or PB2 to enter new password, or press PB3 to enter special "unlocked" state
#define S5 5 // Special unlocked state, do something creative involving clk switching
#define S6 6 // State to enter a new password
#define S7 7 // Error state! check previous state and give an appropriate error message before returning to S0
#define MAX_BLINKS 50
int blinkCount = 0;
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

//uint8_t received_char;
typedef enum {
    LOCKED,
    UNLOCKED,
    FIRSTCORRECT,
    SECONDSCORRECT,
    PASSWORDCORRECT,
    POTENTIOMETERCORRECY,
    ENTERNEWPASSWORD,
    ERRORSTATE,          
} State;
State start_State = LOCKED;

 




int main(void) {
    IOinit(); // set all inputs and outputs, CN interrupts enabled
    clockSetup();// create a 500kHz clock, TMR2 interrupts enabled
    newClk(32);
    InitUART2(); //Setup for UART module to communicate with terminal
    delay_ms(50);
    ADC_setup();
    while(1){
        
        IOcheck();
    }
    return 0;
}


 
 
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
    IFS1bits.CNIF = 0;
    
    return;
}
 
 
 void resetSystem() {
    state = S0;
    start_State = LOCKED;
    Disp2String("System reset. Please start again.");
}
 
 
 
 
void IOcheck(){ 
    switch (start_State) {
        case UNLOCKED:
            Disp2String("Welcome! Enter 1st character:");
            LED_ON;
            received_char = RecvUartChar();
            if (received_char == pass[0]){ 
                start_State = FIRSTCORRECT;
            }
            else{
                start_State = ERRORSTATE;
                Disp2String("Error! Please hit PB2 to restart");       
            }
            break;

        case ERRORSTATE:
            if (PB2state == 1) { 
                resetSystem(); 
            }
            break;                  
    }

    if(start_State == FIRSTCORRECT){
        Disp2String("Correct 1st character. Enter 2nd character:");
        received_char = RecvUartChar();
        while (blinkCount < MAX_BLINKS) {
            LED_ON;         
            delay_ms(250);    //blink at 0.5s interval
            LED_OFF;
            delay_ms(250);  
            blinkCount++;
            if (RXFlag) {
                if(received_char == pass[1]) {
                    start_State = SECONDSCORRECT;
                }
                else{
                    Disp2String("Error: Timeout. Press PB2 to restart.");
                    resetSystem();
                }
                break;
            }
        }
        if (blinkCount >= MAX_BLINKS) {
            Disp2String("Error: Timeout. Press PB2 to restart.");
            resetSystem();
        }
    }   
    
    if (start_State == SECONDSCORRECT){
        
    }
}


//CN interrupt flips global flag variable that main function can read to check which input was given.
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{ 
    // Update PB2state when PB2 is pressed
    if (!PORTBbits.RB4) {
        PB2state = 1;
    } else {
        PB2state = 0;
    }

    IFS1bits.CNIF = 0;  // Clear IF flag
}





