//
//  adc.h
//  IMPVoltmetr
//
//  Created by Radek Pistelak, xpiste04 
//
//  Original (hlavickovy soubor) 
//  -------------------------------------------------
//
//

#ifndef __IMPVoltmetr__adc__
#define __IMPVoltmetr__adc__

#include <stdint.h>

/** Pripravy ADC k pouziti **/
void ADC_init_intref(void);

/** Ziska hodnotu **/
uint16_t getADC(); 

#endif /* defined(__IMPVoltmetr__adc__) */
