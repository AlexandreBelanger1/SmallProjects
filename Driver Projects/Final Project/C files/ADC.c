/*
 * File:   ADC.c
 * Author: Alexandre
 *
 * Created on November 23, 2023, 4:10 PM
 */


#include "xc.h"
#include "Timers.h"
#include "clkChange.h"
#include "Uart.h"
#include "ADC.h"


uint16_t ADCvalue;
void ADC_setup(){
    // Holds ADC converted digital output ADC1BUF0
    uint16_t ADCvalue;

    ADCvalue = 0;
    // ------ ADC initialization --------
    AD1CON1bits.ADON = 1; // Turn on ADC module
    
    // Configure ADC - AD1CON1 bits
    AD1CON1bits.FORM = 0b00; // Data output format bits 
    AD1CON1bits.SSRC = 0b111; // Internal counter ends sampling and starts conversion (auto-convert))
    AD1CON1bits.ASAM = 0; // Sampling begins when SAMP bit is set
    AD1CON1bits.DONE = 0; // Conversion status bit (automatically cleared by MCU once conversion is done)
    
    AD1CON2bits.VCFG = 0b000; // Selects AVDD, AVSS (supply voltage to PIC) as Vref
    
    // Configure ADC - AD1CON2 bits
    AD1CON2bits.VCFG = 0b000; // Reference voltage configuration bits
    AD1CON2bits.CSCNA = 0; // Do not scan inputs
    AD1CON2bits.SMPI = 0b0000; // Interrupts at the completion of conversion for each sample/convert sequence
    AD1CON2bits.BUFM = 0; // Buffer configured as one 16-word buffer
    AD1CON2bits.ALTS = 0; // Always uses MUX A input multiplexer settings
    
    AD1CON3bits.ADRC = 0; // Use system clock
    
    // Configure ADC's time sample - AD1CON3
    // Ensure sample time is 1/10th of signal being sampled or as per
    // application's speed and needs
    AD1CON3bits.SAMC = 0b11111; // 31 * Tad
    AD1CON3bits.ADCS = 0b11111; // 64 * Tcy
    
    // Select and configure ADC input 
    AD1CHSbits.CH0NA = 0; // Channel 0 negative input is Vr-
    AD1CHSbits.CH0SA = 0b0101; // Channel 0 positive input is AN5
    
    AD1PCFG = 0b00000; // Pin configured in analog mode; I/O port read disabled; A/D samples pin voltage
    
    AD1CSSL= 0b00000; // Analog channel omitted from input scan

    // ADC interrupt configuration
    IPC3bits.AD1IP = 7; // Set priority level
    IEC0bits.AD1IE = 1; // Enable timer interrupt
    IFS0bits.AD1IF = 0; // Clear interrupt flag
     
}
uint16_t do_ADC(void) {

    AD1CON1bits.ADON = 1; //start ADC
    AD1CON1bits.SAMP = 1; //start sample
    
    ADCvalue = ADC1BUF0; // ADC output saved in ADC1BUF0 
    return (ADCvalue);  // return 10-bit ADC output 
}

void print_ADC(uint16_t ADCvalue) {
    Disp2String("Voltage meter: ");
    XmitUART2('#', ADCvalue / 36);
    return;
}


void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0; // Clear interrupt flag
    AD1CON1bits.SAMP = 0; // Stop sampling
    AD1CON1bits.ADON = 0; // Turn off ADC and store value in buffer
    
    return;  
    
}

