#
#	Makefile
# 	IJC1
#
#	Created by Radek Pistelak on 25.3.15.
#	Copyright (c) 2015 Radek Pistelak. All rights reserved. 
#
# 	Email: xpiste04@stud.fit.vutbr.cz
#	Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)

# Existuje zpusob jak napsat makefile bez rucniho vypisovani 
# pro dva cile a s podminenym prekladem? :( 

#! Generovani zavislosti: gcc -MM *.c

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -O2

all: prvocisla prvocisla-inline steg-decode

### Ukol a) ###
prvocisla: prvocisla.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -lm prvocisla.o eratosthenes.o error.o -o $@
prvocisla.o: prvocisla.c eratosthenes.h bit-array.h error.h
	$(CC) $(CFLAGS) -c prvocisla.c -o $@	
eratosthenes.o: eratosthenes.c eratosthenes.h bit-array.h error.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@

prvocisla-inline: prvocisla-inline.o eratosthenes-inline.o error.o
	$(CC) $(CFLAGS) -lm prvocisla-inline.o eratosthenes-inline.o error.o -o $@
prvocisla-inline.o: prvocisla.c eratosthenes.h bit-array.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c prvocisla.c -o $@	
eratosthenes-inline.o: eratosthenes.c eratosthenes.h bit-array.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o $@

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o $@

### Ukol b) ###
steg-decode: steg-decode.o ppm.o error.o eratosthenes.o 
	$(CC) $(CFLAGS) -lm steg-decode.o ppm.o error.o eratosthenes.o -o $@
steg-decode.o: steg-decode.c ppm.h error.h eratosthenes.h bit-array.h
	$(CC) $(CFLAGS) -c steg-decode.c -o $@
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o $@

####  

.PHONY: zip clean clean-all

zip: 
	zip xpiste04.zip *.c *.h Makefile

clean:
	rm -f *.o

cclean: 
	rm -f *.o prvocisla prvocisla-inline steg-decode


