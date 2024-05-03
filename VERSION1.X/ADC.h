/*
 * Project: Project_5
 * File:   ADC.h
 * Author: 
 * Created on November 11, 2023, 4: PM
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */


extern uint16_t isZeroPrinted;
//uint16_t ADCvalue;
uint16_t do_ADC(void);
void print_ADC(uint16_t ADCvalue);
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */