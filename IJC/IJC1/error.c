//
//  error.c
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

#include "error.h"

/**
 @brief Funkce vytiskne text "CHYBA: " a potom chybove hlaseni podle formatu fmt.
 @param Stejne parametry jako printf.
 @returns void
 */
void Warning(const char* fmt, ...)
{
        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "CHYBA: ");
        vfprintf(stderr, fmt, arg);
        va_end(arg);
}

/**
 @brief Funkce vytiskne text "CHYBA: " a potom chybove hlaseni podle formatu fmt.
        Funkce UKONCI program volanim exit(1).
 @param Stejne parametry jako printf.
 @returns void
 */
void FatalError(const char* fmt, ...)
{
        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "CHYBA: ");
        vfprintf(stderr, fmt, arg);
        va_end(arg);
        
        exit(1);
}

// End of file: error.c
