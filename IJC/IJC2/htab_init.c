//  
//  htab_init.c
//  IJC2 
// 
//  Created by Radek Pistelak on 21.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 21.4.2015
// 

#include "htable.h"

#include <stdlib.h>

/**
 * @Synopsis Vytvoreni a inicializace HT.
 * @Param size Pocet polozek v ht. 
 * @Returns Ukazatel na inicializovanou HT. 
 */
htab_t* htab_init(unsigned int size) 
{
	if (size == 0) {
		return NULL; 
	}

	htab_t* new = calloc(1, sizeof(htab_t) + size * sizeof(struct htab_listitem));
	if (!new) {
		return NULL; 
	}

	new->htab_size = size; 

	return new;
}

//  End of file: htab_init.c

