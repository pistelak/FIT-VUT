//
//  eratosthenes.c
//  IJC1
//
//  Created by Radek Pistelak on 20.03.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.3.15
//  Priklad: a)
//

#include "eratosthenes.h"

#include <math.h> 

void Eratosthenes(BitArray_t pole)
{
        unsigned long size, limit, multiply;

	size = BA_size(pole);
        limit = (unsigned long) sqrt(size);
     
	// Mini optimalizace: 
	// Vim, ze suda cisla (mimo 2) nepatri mezi prvocisla
	// proto si je muzu dovolit rovnou vyskrtat 
	// a nemusim je kontrolovat v prubehu hlavniho cyklu
	// zbavim se tim podminky...

	// 1 neni prvocisla (narozdil od C poli indexuji od 1) 
	BA_set_bit(pole, 0, 1);

	// suda cisla pryc
	for (unsigned long i = 4; i < size; i+=2) 
	{
		BA_set_bit(pole, i, 1); 
	}

	// zde uz kontroluji jen licha cisla... 
	for(unsigned long i = 3; i <= limit; i+=2) 
	{
                if(BA_get_bit(pole, i) == 0) 
		{
			// opet i zde preskakuji suda cisla...
                        for(unsigned long n = 3; (multiply = n*i) < size; n += 2) 
			{
                                BA_set_bit(pole, multiply, 1);
                        }
                }	
        }
}

// End of file: eratosthenes.c

