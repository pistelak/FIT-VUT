//
//  io.c
//  IJC2 
//
//  Created by Radek Pistelak on 20.04.2015.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.4.2015
//

#include "io.h"

/**
 * @Synopsis Funkce cte jedno slovo o maximalni delce max ze souboru f 
 * 	do zadaneho pole znaku. Pokud je slovo delsi tak do pole zapise
 *	max - 1 znaku a zbytek preskoci. (Vraci skutecnou delku slova 
 *	- bez pocatecnich bilych znaku...)
 *
 * @Param s Ukazatel na pozici kam se bude zapisovat slovo. 
 * @Param max Maximalni pocet znaku zapsaneho slova (vcetne koncove nuly) 
 * @Param f Ukazatel na soubor ze ktereho se bude cist. 
 *
 * @Returns Vraci delku slova (-1 v pripade chyby nebo EOF).
 */
int fgetw(char* s, int max, FILE* f)
{
	if (!s || !f || max < 0) { 
		errno = EINVAL; 
		return -1;
	}

	if (max == 0) {
		return 0;
	}

	int c;  // aktualni znak
	char *init_pos = s; 
	
	for (; s - init_pos < max - 1; ) {
	
		c = fgetc(f);
		
		if (ferror(f)) {
			return -1;
		}
		
		if (isspace(c)) {
		
			if (s == init_pos) { // pocatecni mezery preskakujeme 
				continue;
			} else { // jinak mame nactene slovo
				break;
			}

		} else if (c == EOF) {

			if (s == init_pos) {
				return EOF; //nebyl nacten zadny znak vracim EOF
			} else {
				break;	// jinak si pocka do priste... neutece :)
			}
			
		} else {
			*s++ = c; 
		}
	}

	*s = '\0';

	return s - init_pos;
}

// End of file: io.c

