//  
//  htab_clear.c
//  Nazev projektu
// 
//  Created by Radek Pistelak on 24.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 27.4.2015
// 

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"

static inline void htab_free_item(struct htab_listitem* item);

/**
 * @Synopsis Zruseni vsech polozek v tabulce. 
 * @Param t Hash tabulka
 */
void htab_clear(htab_t* t)
{
	if (!t) {
		return; 	
	}

	struct htab_listitem* item = &t->ptr[0];

	for (unsigned int i = 0; i < t->htab_size; item = &t->ptr[++i]) {
	
		if (item->key) {
			free(item->key);
		}

		if (item->next) {
	
			struct htab_listitem* tmp = item->next;

			for (; tmp->next; tmp = item) {
				item = tmp->next;
				htab_free_item(tmp);
			}

			htab_free_item(tmp);
		}
	}
}


/**
 * @Synopsis  Uvolni polozku v HT
 *
 * @Param item Odkaz na polozku
 */
static inline void htab_free_item(struct htab_listitem* item) 
{
	free(item->key);
	free(item); 
	memset(item, 0, sizeof(struct htab_listitem));
}


//  End of file: htab_clear.c

