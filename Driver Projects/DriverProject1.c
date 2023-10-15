/*
 * File:   main.c
 * Author: Alex
 *
 * Created on September 19, 2023, 8:29 AM
 */


#include "xc.h"
#include <p24F16KA101.h>
int main(void) {
    //Setting inputs
    TRISA = 0x0053; //RA0,RA1,RA4,RA6 as input
    //Setting outputs
    //TRISB = 0x0000;
    TRISB = TRISB|0x4000;

    
    //My UCID is 30037800 (only 8 digits, so I made the 9th digit 0)
    
    while(1){
        if(PORTA == 0b0000000000000000){
            LATB = 0b00000000000000;  //input 0000 gives output of 0 lights
        } else if(PORTA == 0b0000000000000001){
            LATB = 0b0000000000000111;  //input 0001 gives the first digit (most significant) 3
        }
        else if(PORTA == 0b0000000000000010){
            LATB = 0x0000;  //input 0010 gives the second digit, 0
        }
        else if(PORTA == 0b0000000000000011){
            LATB = 0x0000;  //input 0011 gives the third digit, 0
        }
        else if(PORTA == 0b0000000000010000){
            LATB = 0b0000000000000111;  //input 0100 gives the fourth digit, 3
        }
        else if(PORTA == 0b0000000000010001){
            LATB = 0b0000001110010111;  //input 0101 gives the fifth digit, 7
        }
        else if(PORTA == 0b0000000000010010){
            LATB = 0b0001001110010111;  //input 0110 gives the sixth digit, 8
        }
        else if(PORTA == 0b0000000000010011){
            LATB = 0x0000;  //input 0111 gives the seventh digit, 0
        }
        else if(PORTA == 0b0000000001000000){
            LATB = 0x0000;  //input 1000 gives the eighth digit, 0
        }
        else if(PORTA == 0b0000000001000001){
            LATB = 0x0000;  //input 1001 gives the ninth (least significant) digit, 0 (I do not have a 9th digit)
        }
        else{
            LATB = 0x0000;  //all other inputs will output 0 lights
        }
        
    }
    return 0;
}
