//
//  error.h
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

#ifndef __IJC1__error__
#define __IJC1__error__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/**
 @brief Funkce vytiskne text "CHYBA: " a potom chybove hlaseni podle formatu fmt.
 @param Stejne parametry jako printf.
 @returns void
 */
void Warning(const char* fmt, ...);


/**
 @brief Funkce vytiskne text "CHYBA: " a potom chybove hlaseni podle formatu fmt.
        Funkce UKONCI program volanim exit(1).
 @param Stejne parametry jako printf.
 @returns void
 */
void FatalError(const char* fmt, ...);

/*
 Lze napsat pomoci maker:
 
 #define Warning fprintf(stderr, "CHYBA: " __VA_ARGS__);
 #define FatalError fprintf(stderr, "CHYBA: " __VA_ARGS__); exit(1);
*/

#endif /* defined(__IJC1__error__) */

// End of file: error.h
