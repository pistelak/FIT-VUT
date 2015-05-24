//
//  htable.h
//  IJC2 
//
//  Created by Radek Pistelak on 20.04.2015.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.4.2015
//

#ifndef HTABLE_H
#define HTABLE_H

struct htab_listitem {
	char *key; 
	unsigned int data; 
	struct htab_listitem *next;
};

typedef struct {
	unsigned int htab_size;
	struct htab_listitem ptr[];
} htab_t; 

// Rozptylovaci funkce dle zadani. 
unsigned int hash_function(const char* str, unsigned htab_size);

/**
 * @Synopsis Vytvoreni a inicializace HT.
 * @Param size Pocet polozek v ht. 
 * @Returns Ukazatel na inicializovanou HT. 
 */
htab_t* htab_init(unsigned int size);

/**
 * @Synopsis V tabulce t vyhleda odpovidajici zaznam - pokud jej nenalezne, tak jej vytvori.
 * @Param t Hash tabulka
 * @Param key Klic pro vyhledavani v tabulce
 * @Returns Ukazatel na polozku s danym klicem. 
 */
struct htab_listitem* htab_lookup(htab_t* t, const char* key);

/**
 * @Synopsis  Volani funkce pro kazdy prvek v tabulce.
 * @Param t Hash tabulka
 * @Param function Ukazatel na funkci ktera se bude volat
 */
void htab_foreach(htab_t* t, void (*function)(const char* key, unsigned int value));

/**
 * @Synopsis Vyhledani a zruseni dane polozky 
 * @Param t Hash tabulka
 * @Param key Klic identifikujici polozku
 */
void htab_remove(htab_t* t, const char* key);

/**
 * @Synopsis Zruseni vsech polozek v tabulce. 
 * @Param t Hash tabulka
 */
void htab_clear(htab_t* t);

/**
 * @Synopsis  Zruseni cele tabulky (vola htab_clear). 
 * @Param t Hash tabulka
 */
void htab_free(htab_t* t);

/**
 * @Synopsis Tisk prumerne a min/max delky seznamu v tabulce
 * @Param t Hash tabulka
 */
void htab_statistics(htab_t* t);

#endif /* !HTABLE_H */

// End of file: htable.h

