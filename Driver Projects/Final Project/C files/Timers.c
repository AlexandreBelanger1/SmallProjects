//#include <xc.h>
#include "Timers.h"
extern uint16_t timerFlag;
 //Take input of time delay in ms, and set PR2 value to count to specified delay
void delay_ms(uint16_t time_ms){
    
    PR2 = time_ms; //MATH FOR PR2 VALUE: time_ms(desired delay) = PR2*8(prescale)*2*1000/fclk(our 500kHz clock) -> PR2 = times_ms*32000/8*2*1000 -> PR2 = time_ms*(32000/16000) -> PR2 = time_ms*2
    TMR2 = 0;
    T2CONbits.TON = 1;
    
}




void clockSetup(){
    
    //T2CON config
    // Switch clock to 32 kHz (we chose this value because PR2 has a limited range and we need to have a delay of at least 5 seconds on PR2)
    SRbits.IPL = 7;  // Disable interrupts
    CLKDIVbits.RCDIV = 0;  // CLK division = 0
    __builtin_write_OSCCONH(0x66);   // (0x00) for 8MHz; (0x66) for 500kHz; (0x55) for 32kHz;
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN=1;
    while(OSCCONbits.OSWEN==1) //Wait for OSWEN to signal complete before continuing
    {} 
    SRbits.IPL = 0;  //enable interrupts
    
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 3; // set pre-scalar = 8
    T2CONbits.TCS = 0; // use internal clock
    T2CONbits.TSIDL = 0; // operate in idle mode 
    IPC1bits.T2IP = 2; // priority config to 6 (1 priority under CN interrupt)
    IFS0bits.T2IF = 0; // clear interrupt flag
    IEC0bits.T2IE = 1; // enable timer interrupt
    
}   



//T2 Interrupt causes light to switch 1->0 or 0->1
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void){
    IFS0bits.T2IF = 0;
    timerFlag = 1;  
    }
