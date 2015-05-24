//  
//  htab_remove.c
//  IJC2 
// 
//  Created by Radek Pistelak on 21.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 27.4.2015
// 

#include <string.h>
#include <stdlib.h>

#include "htable.h"

/**
 * @Synopsis Vyhledani a zruseni dane polozky 
 *
 * @Param t Hash tabulka
 * @Param key Klic identifikujici polozku
 */
void htab_remove(htab_t* t, const char* key)
{
	if (!t || !key) {
		return; 
	} 

	struct htab_listitem* item = &t->ptr[hash_function(key, t->htab_size)]; 

	if (!item->key) { // polozka v tabulce neni 
		return;
	}

	if (strcmp(item->key, key) == 0) { // mame shodu hned na danem indexu 
		
		if (!item->next) {
			// jedna se o prvni a jedinou polozku
			free(item->key); 
			memset(item, 0, sizeof(struct htab_listitem));
			return;

		} else {
			// jedna se o prvni polozku, ale navazuji na ni i dalsi
			struct htab_listitem* next_item = item->next;
			free(item->key);
			memcpy(item, next_item, sizeof(struct htab_listitem));
			free(next_item);
		}

	} else { // musime hledat hloubeji v seznamu 	

		struct htab_listitem* pre_item;
		
		if (item->next) {
			pre_item = item;
			item = item->next;
		} else { 
			return; // dana polozka v tabulce neni
		}

		for (;;) {
		
			if (strcmp(item->key, key) == 0) {
			
				if (item->next) {
					free(item->key);
					pre_item = item->next; // pre_item bude momentalen "post_item"
					memcpy(item, pre_item, sizeof(struct htab_listitem));
					free(pre_item);				
				} else { // jedna se o posledni polozku v seznamu
					pre_item->next = NULL; 
					free(item->key);
					free(item);	
				}	

				return;
			}	

			pre_item = item;

			if (item->next) {
				item = item->next;
			} else {
				return;
			}
		}
	}
}

//  End of file: htab_remove.c

