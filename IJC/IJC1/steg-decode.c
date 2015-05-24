//
//  steg-decode.c
//  IJC1
//
//  Created by Radek Pistelak on 27.02.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.3.15
//  Priklad: b)
//

#include <ctype.h>

#include "ppm.h"
#include "eratosthenes.h" 

#define N 5000*5000*3

int main(int argc, const char* argv[])
{
	if (argc != 2) {
		FatalError("Neplatne argumenty programu!");
	}

	struct ppm* image = ppm_read(argv[1]);
	if (!image) {
		return EXIT_FAILURE;
	}

	BA_create(bit_array, N); 
	Eratosthenes(bit_array);

	int ch, nbits; 
	ch = nbits = 0; 

	unsigned long image_size = image->xsize * image->ysize * 3;
	for (unsigned long i = 2; i < image_size; i++) {
		
		if (BA_get_bit(bit_array, i) == 0) {

			/** :) kdyz najdeme prvocislo tak to znamena ze 
			 * v poli image->data mame v LSB bitu v bajtu 
			 * na indexu i ulozeny jeden bit znaku zakodovane
			 * zpravy
			 */ 

			// n-ty bit (nbits) znaku (ch) nastavime na hodnotu LSB bitu
			// prvociselne polozky pole image->data
         		DU1_SET_BIT((&ch), nbits, DU1_GET_BIT((&image->data[i]), 0));

			if((++nbits) == CHAR_BIT) { // mame jiz cely znak? 

				if (ch == 0) {  
					break; 
				} else if (isprint(ch)) {
					printf("%c", ch);
					nbits = 0; 
				} else {  
					FatalError("Zprava obsahuje netisknutelny znak!");
				}
			}
		}	
	}

	free(image);

	return EXIT_SUCCESS; 	
}

// End of file: steg-decode.c

