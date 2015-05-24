//
//  prvocisla.c
//  IJC1
//
//  Created by Radek Pistelak on 27.02.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.3.15
//  Priklad: a)
//

#include <stdio.h>
#include <stdlib.h>

#include "eratosthenes.h"

#define N 201000000UL

int main(void)
{
	BA_create(pole, N);

        Eratosthenes(pole);

	// tisk 10 prvocisel v vzestupnem poradi... 
	unsigned long n, i, toPrint[10] = {0};	

	for (n = 10, i = N-1; n > 0 && i > 1; i--)
	{
		if (BA_get_bit(pole, i) == 0) {
			toPrint[--n] = i;
		}
	}

	for (i = 0; i <= 9; i++) 
	{
		printf("%ld\n", toPrint[i]); 
	}

        return 0;
}

