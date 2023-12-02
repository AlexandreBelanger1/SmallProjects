
#include <xc.h>

#include "IOs.h"




int IOcheck(){ 
   //PB1 pressed
    if(PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1){
        return (1);
        
    }
    //PB2 pressed
    else if(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 0){
        return(2);
        
    }
    //PB3 pressed
    else if(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1){
        return(3);
    }
    else {
        return(4);
    }
    return(4);
}


 void IOinit(){
     
    //Setting inputs
    AD1PCFG = 0xFFFF; // Set all analog to digital io 
    AD1PCFGbits.PCFG5 = 0; // Set all analog to digital io 
    TRISBbits.TRISB0 = 0; //Setting input/output pins
    TRISBbits.TRISB1 = 1;
    TRISAbits.TRISA2 = 1; //RA2 as input
    TRISAbits.TRISA4 = 1; //RA4 as input
    TRISAbits.TRISA5 = 1; //RA5 as input
    LATBbits.LATB0 = 1;
    TRISBbits.TRISB4 = 1; //RB4 as input

    
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
    IPC4bits.CNIP = 5; // set priority to highest (1 lowest, 0 off)
    IEC1bits.CNIE = 1; // Enable CN interrupts
    
}
 



