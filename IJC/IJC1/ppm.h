//
//  ppm.h
//  IJC1
//
//  Created by Radek Pistelak on 09.03.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.3.15
//  Priklad: b)
//

#ifndef __IJC1__ppm__
#define __IJC1__ppm__

#include "error.h"

struct ppm {
        unsigned xsize;
        unsigned ysize;
        /* RGB bajty, celkem 3*xsize*ysize */
        char data[];
};

/**
 @brief Nacte obsah PPM souboru do touto funkci dynamicky alokovane struktury.
        Pri chybe formatu pouzije funkci Warning a vrati NULL.
 @param filename        Nazev souboru ze ktereho se bude cist.
 @returns struct ppm* || void
 */
struct ppm* ppm_read(const char* filename);

/**
 @brief Zapise obsah struktury do souboru.
 @param p               ukazatel na PPM strukturu
        filename        Nazev souboru do ktereho se bude zapisovat.
 @returns 0 || -1 (pri chyba -> upozorni pomoci fce Warning)
 */
int ppm_write(struct ppm* p, const char *filename);


#endif /* defined(__IJC1__ppm__) */

// End of file: ppm.h
