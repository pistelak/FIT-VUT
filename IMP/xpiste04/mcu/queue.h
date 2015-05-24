//
//  queue.h
//  IMPVoltmetr
//
//  Created by Radek Pistelak, xpiste04.
//  Original
//  -------------------------------------------------
//
//  Jednoduchy kruhovy buffer. // Fronta
//

#ifndef __IMPVoltmetr__queue__
#define __IMPVoltmetr__queue__

#include <stdint.h>
#include <stdbool.h>


/* Delka bufferu odpovida 75 procentum obrazovky  */
#define LEN     60



/* Datovy typ fronty - kruhoveho bufferu */

typedef struct {
	/* Vlastni data */
	uint16_t data[LEN];
	/* Index na prvni prvek */
	uint8_t start;
	/* Index na posledni prvek */
	uint8_t end;
} queue_t;

/**
 * Pro zjednoduseni nechame frontu jako globalni promennou.
 */
queue_t Q;

/**
 * @brief Inicializuje frontu
 * @return void
 */
static inline void queueInit()
{
        Q.start = 0;
        Q.end = 0;
}

static inline bool queueIsFull() {
	return (Q.end + 1) % LEN == Q.start;
}


/**
 * @brief Prida do fronty prvek point
 * @param Prvek ktery se ma pridat do fronty
 * @return void
 */
static inline void queueAddPoint(uint16_t point)
{
        Q.data[Q.end] = point;
        Q.end = ++Q.end % LEN;
        
        if (Q.end == Q.start) {
                Q.start = (Q.start + 1) % LEN;
        }
}

/**
 * @brief Vrati prvek na indexu index.
 * @param       index   Index ze ktereho se bude cist.
 * @return              Data ulozena na danem prvku.
 */
static inline uint16_t queueReadPoint(uint8_t index)
{
        if (index >= LEN) return 0;
        
        return Q.data[(Q.start + index) % LEN];
}

#endif /* defined(__IMPVoltmetr__queue__) */
