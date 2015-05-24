//
//  bit-array.h
//  IJC1
//
//  Created by Radek Pistelak on 02.03.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Datum posledni zmeny: 27.3.15
//  Priklad: a)
//	
//  Makro CONTROL_INDEX (l:88) zpusobuje pri prekladu na serveru merlin (gcc 4.8.4)
//  varovne hlaseni. 
//  

#ifndef IJC1_bit_array_h
#define IJC1_bit_array_h

#include "error.h"

#include <limits.h>  // pro CHAR_BIT

typedef unsigned long BitArray_t[];

/**
 * SET_BIT a GET_BIT
 * -------   -------
 */

// pro lepsi citelnost... 
#define MY_SET_BIT(x, n, b) \
	x ^= (-b ^ x) & (1UL << n)

#define MY_GET_BIT(x, n) \
	( (x >> n) & 1UL )

#define DU1_SET_BIT(p, i, b) \
	MY_SET_BIT((*p), (i % (CHAR_BIT * sizeof(*p))), (b))

#define DU1_GET_BIT(p, i) \
	MY_GET_BIT((*p), (i % (CHAR_BIT * sizeof(*p))))

/**
 * Vytvori bitove pole a nastavi prvni prvek na maximalni index pole a zbytek nuluje
 * -------------------
 */

// Pocet bitu v unsigned long
#define UL_BIT \
        (CHAR_BIT * sizeof(unsigned long))

// Zjisti potrebnou velikost pole pro zadany pocet bitu
#define BA_compute_size(pocet_bitu) \
        ((pocet_bitu % UL_BIT == 0) ? ((pocet_bitu / UL_BIT) + 1) : ((pocet_bitu / UL_BIT) + 2))

#define BA_create(jmeno_pole, velikost) \
        unsigned long jmeno_pole[BA_compute_size(velikost)] = {velikost, 0}

/**
 * Vrati velikost pole -> prvni prvek
 * -------------------
 */
#ifdef USE_INLINE
        static inline unsigned long BA_size(BitArray_t jmeno_pole)
        {
                return jmeno_pole[0];
        }
#else
        #define BA_size(jmeno_pole) \
                ((unsigned long) jmeno_pole[0])
#endif

/**
 * Nastavi zadany bit v poli na hodnotu zadanou vyrazem (kontroluje meze poli)
 * ----------------------------------------------------
 */

#ifdef USE_INLINE
        static inline void BA_set_bit(BitArray_t jmeno_pole, unsigned long index, unsigned long vyraz)
        {
                if (index >= BA_size(jmeno_pole)) {
                        FatalError("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index,
				       	(unsigned long) BA_size(jmeno_pole));
                } else {
                        DU1_SET_BIT(&jmeno_pole[index/UL_BIT+1], index, vyraz);
                }
	}
#else
	// prekladac gcc 4.8.4 (merlin) vypisuje warning, ze podminka 
	// index >= 0 bude vzdy splnena, protoze se jedna o unsigned long 
	// nicmene makra nekontroluji typy, 
	// tak ji tu pro jistotu necham (kdyby se nekdo pokousel o 
	// nejake podivne ouziti...) :/ 
        #define CONTROL_INDEX(velikost, index, X) \
		(((velikost) > (index) && (index) >= 0) ? (X) : \
                        (FatalError("Index %lu mimo rozsah 0..%lu\n", (unsigned long)(index), \
				(unsigned long)(velikost)), 0))

        #define BA_set_bit(jmeno_pole, index, vyraz) \
                CONTROL_INDEX(BA_size(jmeno_pole), index, \
                        DU1_SET_BIT(&jmeno_pole[index/UL_BIT+1], index, vyraz) \
                )

#endif

/**
 * Ziska hodnotu zadaneho bitu, vraci hodnotu 0 nebo 1 (kontroluje meze poli)
 * ---------------------------------------------------
 */
#ifdef USE_INLINE
        static inline unsigned BA_get_bit(BitArray_t jmeno_pole, unsigned long index)
        {
                if (index >= BA_size(jmeno_pole)) {
                        FatalError("Index %lu mimo rozsah 0..%lu\n", (unsigned long) index, 
					(unsigned long) BA_size(jmeno_pole));
                }
               
                return DU1_GET_BIT(&jmeno_pole[index/UL_BIT+1], index);
        }
#else 
        // vyuziva makro CONTROL_INDEX, ktere je uvedeno u makra BA_set_bit
                
        #define BA_get_bit(jmeno_pole, index) \
                (CONTROL_INDEX(BA_size(jmeno_pole), index, \
                        DU1_GET_BIT(&jmeno_pole[index/UL_BIT+1], index) \
                ))
#endif

#endif

// End of file: bit-array.h

