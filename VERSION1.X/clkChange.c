/*
 * File:   clkChange.c
 * Author: shanz
 *
 * Created on December 2, 2023, 11:32 AM
 */


#include "xc.h"
#include "clkChange.h"

void newClk(unsigned int clkval) {
    char COSCNOSC;
    switch(clkval) {
        case 8: // 8 MHz
            COSCNOSC = 0x00;
            break;
        case 500: // 500 kHz
            COSCNOSC = 0x66;
            break;
        case 32: // 32 kHz
            COSCNOSC = 0x55;
            break;
        default:
            COSCNOSC = 0x55;
    }
    SRbits.IPL = 7;
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(COSCNOSC);
    __builtin_write_OSCCONL(0x01);
//    OSCCONbits.OSWEN = 1;
    while(OSCCONbits.OSWEN==1) {}
    SRbits.IPL = 0;
}