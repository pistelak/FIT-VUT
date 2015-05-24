//
//  ppm.c
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#include "ppm.h"

#define E_OK 	0
#define E_ERR 	-1

#define IMAGE_SIZE(p) 3 * p->xsize * p->ysize

/**
 @brief Nacte obsah PPM souboru do touto funkci dynamicky alokovane struktury.
        Pri chybe formatu pouzije funkci Warning a vrati NULL.
 @param filename        Nazev souboru ze ktereho se bude cist.
 @returns struct ppm* || void
 */
struct ppm* ppm_read(const char* filename)
{
	if (!filename) return NULL; 
	
        struct ppm* image = (struct ppm *) malloc(sizeof(struct ppm)); 
	if (!image) {
		FatalError("Alokace pameti se nezdarila!");
	}

	FILE* fd = fopen(filename, "rb");
	if (!fd) {
		free(image);
		FatalError("Nepodarilo se otevrit soubor!");
	}

	unsigned max_value;  // pomocne promenne (viz. specifikace formatu) 
	char buffer[2]; 
	if (fscanf(fd, "%2s %u %u %u%*c", buffer, &image->xsize, &image->ysize, &max_value) != 4) {
		// nebyly nacteny hodnoty xsize, ysize nebo max_value
		goto err_fmt; 
	}

	if (image->xsize > 5000 || image->ysize > 5000 || max_value > 255 || strncmp("P6", buffer, 2)) {
		// porusuje implementacni limit (5000x5000x3) nebo nesedi max hodnota 
		// pripadne neodpovida typ binarnimu kodovani formatu
		goto err_fmt; 
	}	

	image = (struct ppm*) realloc(image, sizeof(struct ppm) + IMAGE_SIZE(image) * sizeof(char));
	if (!image) {
	 	fclose(fd); 
		FatalError("Alokace pameti se nezdarila!");
	}

	if(fread(&image->data, sizeof(char), IMAGE_SIZE(image), fd) != IMAGE_SIZE(image)) {	
		goto err_fmt; 
	}

	fclose(fd);

        return image;

err_fmt: 
	Warning("Chyba formatu"); 
	fclose(fd);
	free(image); 
	return NULL;
}

/**
 @brief Zapise obsah struktury do souboru.
 @param p               ukazatel na PPM strukturu
        filename        Nazev souboru do ktereho se bude zapisovat.
 @returns 0 || -1 (pri chyba -> upozorni pomoci fce Warning)
 */
int ppm_write(struct ppm* p, const char* filename)
{
	if (!p || !filename) {
		return E_ERR;
	}

	FILE *fd = fopen(filename, "wb");
	if (!fd) {
		Warning("Nepodarilo se otevrit soubor: %s", filename);
	}

	// zapis formatovacich dat	
	if (fprintf(fd, "P6\n%u %u\n%u\n", p->xsize, p->ysize, 255) < 0) {
		goto err_wrt; 
	}

	// zapis obrazovych dat 
	if (fwrite(p->data, sizeof(char), IMAGE_SIZE(p), fd) != IMAGE_SIZE(p)) {
		goto err_wrt; 
	}
	
	fclose(fd);

	return E_OK; 

err_wrt:
	Warning("Chyba zapisu do souboru: %s!", filename);
	fclose(fd); 
	return E_ERR; 
}

#undef IMAGE_SIZE


// End of file: ppm.h

