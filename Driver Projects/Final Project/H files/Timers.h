// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef Timers
#define	Timers
#include "xc.h"
 // include processor files - each processor file is guarded.  


void delay_ms(uint16_t);
void clockSetup(void);
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void);


#endif	/* Timers */