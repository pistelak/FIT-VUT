
//
// Radek Pistelak, xpiste04@stud.fit.vutbr.cz
//
// Prevzato z:
// http://320volt.com/wp-content/uploads/2010/10/msp430x54x_adc12_07.c.html
// 
// Vlastni kod = 10% (prizpusobeno fitkitu + prevod vysledku ADC na mV) 
//

#include "adc.h"
#include <msp430x16x.h>
#include <fitkitlib.h>
#include <signal.h>


volatile unsigned int result;

void ADC_init_intref(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL |= 0x01;                            // Enable A/D channel A0
  ADC12CTL0 = ADC12ON+SHT0_8+MSC;           // Turn on ADC12, set sampling time
                                            // set multiple sample conversion
  ADC12CTL1 = SHP + CONSEQ_2;               // Use sampling timer, set mode
  ADC12IE = 0x01;                           // Enable ADC12IFG.0
  ADC12CTL0 |= ENC;                         // Enable conversions
  ADC12CTL0 |= ADC12SC;                     // Start conversion
 
  _EINT(); 
}

interrupt (ADC12_VECTOR) interruptHandler (void)
{
	static unsigned char index = 0;

  	switch(ADC12IV)
  	{
		case  0: break;                           // Vector  0:  No interrupt
		case  2: break;                           // Vector  2:  ADC overflow
		case  4: break;                           // Vector  4:  ADC timing overflow
		case  6:                                  // Vector  6:  ADC12IFG0
		   result = ADC12MEM0;
		case  8: break;                           // Vector  8:  ADC12IFG1
		case 10: break;                           // Vector 10:  ADC12IFG2
		case 12: break;                           // Vector 12:  ADC12IFG3
		case 14: break;                           // Vector 14:  ADC12IFG4
		case 16: break;                           // Vector 16:  ADC12IFG5
		case 18: break;                           // Vector 18:  ADC12IFG6
		case 20: break;                           // Vector 20:  ADC12IFG7
		case 22: break;                           // Vector 22:  ADC12IFG8
		case 24: break;                           // Vector 24:  ADC12IFG9
		case 26: break;                           // Vector 26:  ADC12IFG10
		case 28: break;                           // Vector 28:  ADC12IFG11
		case 30: break;                           // Vector 30:  ADC12IFG12
		case 32: break;                           // Vector 32:  ADC12IFG13
		case 34: break;                           // Vector 34:  ADC12IFG14
		default: break;
  }
}


/*
 * Prevede vysledek z ADC na mV 
 * Neopisovat - je to blbost... ;) 
 */
uint16_t getADC(void)
{
	return result * 5 / 8;
}



