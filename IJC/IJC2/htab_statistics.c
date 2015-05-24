//  
//  htab_statistics.c
//  IJC2 
// 
//  Created by Radek Pistelak on 24.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 27.4.2015
// 

#include <stdio.h>

#include "htable.h"

/**
 * @Synopsis Tisk prumerne a min/max delky seznamu v tabulce
 * @Param t Hash tabulka
 */
void htab_statistics(htab_t* t)
{
	if (!t) {
		return;
	}

	unsigned int n, min, max, avg;
	struct htab_listitem* item;

	n = max = avg = 1;
	min = -1; // nastavim min na nejvetsi moznou hodnotu...

	for (unsigned int i = 0; i < t->htab_size; i++) {
	
		item = &t->ptr[i];

		if (!item->next) {
			avg++;
			min = 1;
		} else {

			for (n = 1; item->next; n++) {
				item = item->next;
			}

			min = (n < min) ? n : min;
			max = (n > max) ? n : max;
			avg += n;
		}
	}

	printf("min:%u \t max: %u \t avg: %u\n", min, max, avg/t->htab_size);
}

//  End of file: htab_statistics.c

