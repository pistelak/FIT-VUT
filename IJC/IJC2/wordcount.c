//  
//  wordcount.c
//  Nazev projektu
// 
//  Created by Radek Pistelak on 25.04.2015. 
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 27.4.2015
// 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "htable.h"
#include "io.h"

#define BUFFER_SIZE 127

/** 
 * Zvolit spravnou velikost je slozite, kdybychom implementovali
 * tabulku s otevrenym adresovanim (open adressing), tak je volba 
 * jednodussi jelikoz ji muzeme pri urcitem pomeru obsazeni 
 * zvetsit (resi se treba zde: 
 * http://www.algoritmy.net/article/32077/Hashovaci-tabulka). 
 * 
 * Nicmene tak kdyz neznam testovaci sadu, tak je to loterie...
 * Volim tedy 1117 (jako prvocislo).
 * 
 * Otestovano na http://www.gutenberg.org/cache/epub/1661/pg1661.txt
 * (The Adventures of Sherlock Holmes) s prumernou delkou seznamu 13. 
 */
#define HT_SIZE 1117

void print(const char* str, unsigned int data) 
{
	printf("%s\t%u\n", str, data); 
}

int main(void) 
{
	char buffer[BUFFER_SIZE];
	bool warning = false; // vypsani varovneho hlaseni (pri prekroceni impl. limitu)
	
	htab_t* t = htab_init(HT_SIZE);
	if (!t) {
		perror("ERR: ");
		return EXIT_FAILURE;
	}

	int count; // pocet nactenych znaku
	struct htab_listitem* item; 

	while ((count = fgetw(buffer, 127, stdin)) != EOF) {
	
		if (count > 127 && !warning) {
			warning = true; 
			fprintf(stderr, "ERR: Prekrocen implementacni limit delky slova!\n");	
		}

		item = htab_lookup(t, buffer); 
		if (!item) {
			perror("ERR: ");
			htab_free(t);
			return EXIT_FAILURE;
		}

		item->data += 1;
	}

	if (errno) {
		perror("ERR: ");
		htab_free(t);
		return EXIT_FAILURE;
	}

	htab_foreach(t, print);

	// htab_statistics(t);

	htab_free(t);

	return EXIT_SUCCESS;
}

//  End of file: wordcount.c

