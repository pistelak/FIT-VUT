/*******************************************************************************
	main.c

	Radek Pistelak, xpiste04

	Prevzaty a upraveny main ze hry snake (80%)

*******************************************************************************/

#include <fitkitlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcd/display.h>
#include <stdint.h>

#include "print.h"
#include "adc.h"
#include "vga_block.h"

// neni implementovana zvlastni funkcionalita
// staci tedy pouze projekt - autor
void print_user_help(void)
{
    	term_send_str_crlf("IMPVoltmetr - xpiste04");
}


unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
	return '\0';
}

// Inicializace FPGA a vypis na LCD fitkitu
void fpga_initialized(void)
{
  	LCD_init();
	VRAM_Init();
}

int main()
{
	initialize_hardware();

	// buffer pro vypis na LCD
	char buffer[100];

	// stop
	WDTCTL = WDTPW + WDTHOLD;

	// init ADC
	term_send_str_crlf("Init ADC");
	ADC_init_intref();
	term_send_str_crlf("OK");

	/*
	 * S prodlevou 30ms bude opakovane
	 * zadat ADC o dalsi hodnotu,
	 * kterou potom pomoci funkce reDraw
	 * vykresli na obrazovku
	 */
  	while(1)
	{
		unsigned int new = getADC();

		// vypise hodnotu na LCD fitkitu
		sprintf(buffer,"%u mV", new * 2);
		LCD_write_string(buffer);

		reDraw(new / VGA_ROWS);

		delay_ms(30);
  	}

	LCD_write_string("xpiste04");

 	return 0;
}
