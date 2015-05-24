//  
//  hash_function.c
//  IJC2
// 
//  Created by Radek Pistelak on 21.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 28.4.2015
// 

#include <stdio.h>

#if 0
// Rozptylovaci funkce dle zadani. 

unsigned int hash_function(const char* str, unsigned htab_size)
{
	unsigned int h = 0;
	const unsigned char* p;

	for (p = (const unsigned char*) str; *p != '\0'; p++) {
		h = 65599 * h + *p;
	}

	printf("hash1\n");

	return h % htab_size; 
}

#else 

unsigned int hash_function(const char* str, unsigned htab_size)
{
	unsigned long hash = 5381;
        int c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	printf("hash2\n");

        return hash % htab_size;
}

#endif

//  End of file: hash_funcion.c

