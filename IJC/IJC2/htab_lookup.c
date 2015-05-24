//  
//  htab_lookup.c
//  IJC2 
// 
//  Created by Radek Pistelak on 21.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 27.4.2015
// 

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "htable.h"

static inline int create_string(char **dst, const char* src);
static inline int ht_new_item(struct htab_listitem** dst, const char* key);

/**
 * @Synopsis V tabulce t vyhleda odpovidajici zaznam - pokud jej nenalezne, tak jej vytvori.
 *
 * @Param t Hash tabulka
 * @Param key Klic pro vyhledavani v tabulce
 *
 * @Returns Ukazatel na polozku s danym klicem. 
 */
struct htab_listitem* htab_lookup(htab_t* t, const char* key) 
{
	if (!t || !key)	{
		errno = EINVAL;
		return NULL;
	}

	unsigned int index = hash_function(key, t->htab_size);
	struct htab_listitem* ht_record = &t->ptr[index]; 

	if (!ht_record->key) { // pokud na danem indexu jeste nic neni: 
		
		if (create_string(&ht_record->key, key) != 0) {
			return NULL; 
		}

	} else { 
		
		for (; ; ht_record = ht_record->next) { // prohledame seznam zda uz polozka neexistuje
		
			if (strcmp(ht_record->key, key) == 0) {
				return ht_record;
			}
			
			if (!ht_record->next) break;
		}

		if (ht_new_item(&ht_record->next, key) != 0) { // pokud ne tak ji vytvorime
			return NULL;
		}
		
		ht_record = ht_record->next;
	}

	return ht_record; 
}

/**
 * @Synopsis  Vytvori C-string (alokuje pamet a nakopiruje zdrojovy retezec).
 *
 * @Param dst Ukazatel na umisteni noveho retezce
 * @Param src Retezec ktery se bude "duplikovat"
 *
 * @Returns  0 - OK, -1 - ERR 
 */
static inline int create_string(char **dst, const char* src) 
{
	size_t len = strlen(src) + 1;

	char *new = (char *) malloc (sizeof(char) * len); 
	if (!new) {
		errno = ENOMEM;
		return -1; 
	}

	strncpy(new, src, len); 

	*dst = new; 

	return 0;
}

/**
 * @Synopsis Vytvori novou polozku ht  
 *
 * @Param dst Umisteni nove polozky 
 * @Param key Klic 
 *
 * @Returns 0 - OK, -1 - ERR 
 */
static inline int ht_new_item(struct htab_listitem** dst, const char* key) 
{

	struct htab_listitem* new = (struct htab_listitem*) calloc (1, sizeof(struct htab_listitem));
	if (!new) {
		errno = ENOMEM;
		return -1;
	}

	if (create_string(&(new->key), key) != 0) {
		free(new);
		return -1; 
	}

	*dst = new; 

	return 0;
}

//  End of file: htab_lookup.c

