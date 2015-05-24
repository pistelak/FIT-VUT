//
//  tail.c
//  IJC2
//
//  Created by Radek Pistelak on 15.02.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 28.4.2015
//
//  -------------------------------------------------------
//
//  - Implementace je zalozena na kruhovem bufferu do ktereho
//  jsou postupne nacitany vsechny radky (plati pro
//  vypis poslednich n radku).
//
//  - Program je schopny pracovat s libovolne dlouhymi radky...
//
//  - Urcite by to slo napsat efektivneji jak do delky kodu, 
//  tak efektivity, ale tohle by melo taky fungovat...
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

#define NDEBUG 
#include <assert.h>

#define N 10

#define NEW(T)		((T *) malloc (sizeof (T)))
#define CNEW(T, C)	((T *) calloc ((C), sizeof (T)))
#define RESIZE(T, V, S)	((T *) realloc ((void *)(V), (S)))

// Struktura pro praci s parametry
struct params_s {
        /* pocet radku || od ktereho radku */
        uintmax_t n;
        /* nazev souboru */
        char *file;
        /* plus -> od ktereho radku */
        bool plus;
};

int get_params(int argc, char* argv[], struct params_s* p);

// Struktura reprezentujici radek kruhoveho bufferu 
struct line_s {
        /* jeden radek */
        char* str;
        /* aktualne alokovany prostor */
        uintmax_t size;
};

// Struktura kruhoveho bufferu
struct buffer_s {
        /* vlastni data */
        struct line_s* lines;
        /* pozice prvniho radku */
        uintmax_t start;
        /* pozice posledniho radku */
        uintmax_t end;
        /* pocet radku */
        uintmax_t n;
	/* pocet nactenych radku */
	uintmax_t readed;
};

struct buffer_s* buffer_create(const uintmax_t lines);
void buffer_destroy(struct buffer_s* buffer);
static inline struct line_s* buffer_actual_line(struct buffer_s* buffer);
static inline void buffer_next_line(struct buffer_s *buffer);
static inline intmax_t buffer_read_line(struct buffer_s* buffer, FILE* fp);
static inline void buffer_print_lines(struct buffer_s* buffer);

/**
 * @Synopsis  Nacte radek ze souboru. (V pripade potreby se 
 * 	funkce sama postara o velikost dst)
 *
 * @Param dst  Uk. na retezec do ktereho se bude nacitat. 
 * @Param size  Velikost bufferu
 * @Param fp  Ukazatel na soubor, ze ktereho se bude cist.
 * 
 * @Returns  ERR: -1, EOF nebo pocet nactenych znaku 
 */
intmax_t fgetline(char **dst, uintmax_t* size, FILE *fp)
{
	assert(dst && fp && size);

	if (*size == 0 || !*dst) { 
                *size = 512;
		*dst = RESIZE(char, *dst, *size * sizeof(char));
		if (!*dst) {
			return -1;
		}
	}
      
        char *actual_char = *dst;

        for (uintmax_t i = 0; ; i++) {
               
                int c = fgetc(fp); // aktualni znak
               
		if (ferror(fp)) {
			return -1;
		}

                if (*size == i + 1) {
                        *size *= 2;
			*dst = RESIZE(char, *dst, *size * sizeof(char));
			if (!*dst) {
				return -1;
			}
			actual_char = *dst + i * sizeof(char);
                }
               
                switch (c) {
                        case EOF:
                                return EOF;
                        case '\n':
        			*actual_char = '\0';
                                break;
                        default:
                                *actual_char++ = c;
                                continue;
                }
                break;
        }

	return actual_char - *dst;
}

/**
 * @Synopsis  Vytiskne poslednich n radku ze souboru. 
 * @Param file  Nazev souboru se kterym se bude pracovat.
 * @Param n  Pocet pozadovanych radku.
 * @Returns  ERR: -1, OK: 0
 */
int print_last_n_lines(FILE* fp, const uintmax_t n)
{
	assert(fp && n > 0);

	struct buffer_s* b = buffer_create(n);
        if (!b) {
		perror(__func__);
		return -1;
        }

	// nactu po radcich soubor do bufferu
	while (buffer_read_line(b, fp) >= 0); 

	if (errno) {
		perror(__func__);
		buffer_destroy(b);
		return -1;
	}

	// tisknu buffer
	buffer_print_lines(b);

	buffer_destroy(b);

        return 0;
}

/**
 * @Synopsis  Vytiskne soubor od n-teho radku.
 * @Param file Nazev souboru. 
 * @Param n  Cislo radku od ktereho se bude tisknout.
 * @Returns ERR: -1, OK: 0 
 */
int print_from_n_line(FILE* fp, uintmax_t n)
{
	assert(fp);

	int c;

	while (n > 0 && (c = fgetc(fp)) != EOF) { // preskocim n radku
		if (c == '\n') --n;
	}

	while ((c = fgetc(fp)) != EOF) { // zbytek vypisu
		putchar(c);
	}

	return 0;
}

/**
 * @Synopsis  Vytvori kruhovy buffer o zadanem poctu radku.
 * @Param lines Pocet radku
 * @Returns Ukazatel na buffer 
 */
struct buffer_s* buffer_create(const uintmax_t lines)
{
	assert(lines > 0);

	struct buffer_s* buffer = NEW(struct buffer_s);
        if (!buffer) {
                return NULL;
        }
     
	buffer->lines = CNEW(struct line_s, lines); 
        if (!buffer->lines) {
                free(buffer);
                return NULL;
        }

        buffer->n = lines;
        buffer->end = buffer->start = buffer->readed = 0;
        
        return buffer;
}

/**
 * @Synopsis  Vrati aktualni radek bufferu.
 * @Param buffer Buffer
 * @Returns Ukazatel na aktualni radek
 */
static inline struct line_s* buffer_actual_line(struct buffer_s* buffer)
{
	assert(buffer);

	return &buffer->lines[buffer->end]; 
}

/**
 * @Synopsis  Nastavi v bufferu dalsi radek.
 * @Param buffer Buffer
 */
static inline void buffer_next_line(struct buffer_s* buffer) 
{
	assert(buffer);

	buffer->end = ++buffer->end % buffer->n; 

	if (buffer->end == buffer->start) {
		buffer->start = (buffer->start + 1) % buffer->n;
	}
}

/**
 * @Synopsis  Nacte do bufferu radek textu.
 * @Param  buffer Buffer
 * @Param fp  Ukazatel na soubor.
 * @Returns  Vysledek operace fgetline 
 */
static inline intmax_t buffer_read_line(struct buffer_s* buffer, FILE* fp) 
{
	assert(buffer && fp);
	
	intmax_t status = fgetline(&buffer_actual_line(buffer)->str,
				   &buffer_actual_line(buffer)->size,
				   fp);

	if (status > 0) {
		buffer_next_line(buffer);
		buffer->readed++; 
	}
	
	return status;
}

/**
 * @Synopsis  Vytiskne obsah bufferu.
 * @Param buffer Buffer.
 */
static inline void buffer_print_lines(struct buffer_s* buffer) 
{
	assert(buffer);

	uintmax_t toPrint = (buffer->readed < buffer->n) ? buffer->readed : buffer->n;

	for (uintmax_t i = 0; i < toPrint; i++) {
		printf("%s\n", buffer_actual_line(buffer)->str);
		buffer_next_line(buffer);
	}
}

/**
 * @Synopsis  Uvolni pamet po bufferu.
 * @Param buffer Buffer.
 */
void buffer_destroy(struct buffer_s* buffer)
{
	assert(buffer);

        for (uintmax_t i = 0; i < buffer->n; i++) {
		if (buffer->lines[i].str) 
		{
			free(buffer->lines[i].str);
		}
        }
     
        free(buffer->lines);
        free(buffer);
}

/**
 * @Synopsis  Zpracuje parametry programu zadane na prik. radce.
 * @Param argc  Pocet parametru
 * @Param argv[]  Text jednotlivych parametru
 * @Param p  Ukazatel na strukturu do ktere se ulozi vysledek
 * @Returns  ERR: -1, OK: 0
 */
int get_params(int argc, char* argv[], struct params_s* p)
{
	char *num_ptr;
	char *end_ptr; 

	if (argc == 1) { // vse default
		return 0;
		
	} else if (argc == 2) { // soubor
		p->file = argv[1];
		
	} else if (argc == 3 || argc == 4) { // parametry || parametry a soubor

		if (argv[1][0] == '-' && argv[1][1] == 'n') { // -n	
			
			num_ptr = argv[2]; // ukazatel na cislo 
			
			if (*num_ptr == '+') {
				p->plus = true; 
				++num_ptr;
			}
			
                        p->n = strtoumax(num_ptr, &end_ptr, 10);

			if (*end_ptr != '\0') {
				return -1;
			}

		} else {
			return -1;
		}

		if (argc == 4) {
			p->file = argv[3]; 
		}	

	} else {
		return -1;
	}

        return 0;
}


int main(int argc, char* argv[])
{
        struct params_s p = { .n = N, .file = NULL, .plus = false };

        if (get_params(argc, argv, &p)) {
                fprintf(stderr, "ERR: Neplatne parametry!\n");
                return EXIT_FAILURE;
        }

	FILE* fp;

	if (p.file) { // zjistime odkud budeme cist vstupni data
		fp = fopen(p.file, "r"); 
		if (!fp) {
			perror(__func__);
			return EXIT_FAILURE; 
		}
	} else {
		fp = stdin;
	}

	int status;

	if (p.plus) {
		status = print_from_n_line(fp, p.n);
	} else {
        	status = print_last_n_lines(fp, p.n);
	}

	if (p.file) {
		fclose(fp);
	}

	return status; 
}

//  End of file: tail.c

