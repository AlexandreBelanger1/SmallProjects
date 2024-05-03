
#ifndef Timers
#define	Timers
#include "xc.h"
 // include processor files - each processor file is guarded.  


void delay_ms(uint16_t time_ms);
void delay_msT2(uint16_t time_ms);
void delay_msT3(uint16_t time_ms);
void clockSetup(void);
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv))_T1Interrupt(void);
void __attribute__((interrupt, no_auto_psv))_T3Interrupt(void);
#endif	/* Timers */