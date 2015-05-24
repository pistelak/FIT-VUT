//  
//  htab_foreach.c
//  IJC2 
// 
//  Created by Radek Pistelak on 24.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 28.4.2015
// 

#include <stdint.h>

#include "htable.h"

/**
 * @Synopsis  Volani funkce pro kazdy prvek v tabulce.
 * @Param t Hash tabulka
 * @Param function Ukazatel na funkci ktera se bude volat
 */
void htab_foreach(htab_t* t, void (*function)(const char* key, unsigned int value))
{
	if (!t || !function) {
		return;
	}
	
	for (unsigned int i = 0; i < t->htab_size; i++) { // cela HT
	 	
		for (struct htab_listitem* item = &t->ptr[i];  ; item = item->next) { // jednotlive seznamy
			      	
			if (item->key) {
				function(item->key, item->data);
			}

			if (!item->next) break;
		}
	}
}

//  End of file: htab_foreach.c

