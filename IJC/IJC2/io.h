//
//  io.h
//  IJC2 
//
//  Created by Radek Pistelak on 20.04.2015.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.4.2015
//

#ifndef IO_H
#define IO_H

#include <stdio.h> 
#include <ctype.h>
#include <errno.h> 
#include <stdbool.h>

/**
 * @Brief Funkce cte jedno slovo o maximalni delce max ze souboru f 
 * 	do zadaneho pole znaku. (Pokud je slovo delsi tak do pole zapise
 *	max - 1 znaku a zbytek preskoci.) 
 *
 * @Param s Ukazatel na pozici kam se bude zapisovat slovo. 
 * @Param max Maximalni pocet znaku zapsaneho slova (vcetne koncove nuly) 
 * @Param f Ukazatel na soubor ze ktereho se bude cist. 
 *
 * @Returns Vraci delku slova (EOF v pripade konce souboru a -1 v pripade chyby).
 */
int fgetw(char* s, int max, FILE* f);

#endif /* !IO_H */

// End of file: io.h

