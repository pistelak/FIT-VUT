//  
//  htab_free.c
//  IJC2 
// 
//  Created by Radek Pistelak on 24.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 28.4.2015
// 

#include <stdlib.h>

#include "htable.h"

/**
 * @Synopsis  Zruseni cele tabulky (vola htab_clear). 
 * @Param t Hash tabulka
 */
void htab_free(htab_t* t)
{
	if (!t) {
		return;
	}

	htab_clear(t); // uvolni vsechny polozky v tabulce 
	free(t);
}

//  End of file: htab_free.c

