#
#  Makefile
#  IJC2
#
#  Created by Radek Pistelak on 28.3.15.
#  Copyright (c) 2015 Radek Pistelak. All rights reserved. 
#
#  Email: xpiste04@stud.fit.vutbr.cz
#  Prekladac: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
#
#

CC = gcc
CXX = g++
CFLAGS = -std=c99 -O2 -Wall -Wextra -pedantic
CXXFLAGS = -std=c++11 -O2 -Wall -pedantic

SNAME = libhtable.a
DNAME = libhtable.so

HTAB_SRC = $(wildcard h*.c)
HTAB_OBJ = $(HTAB_SRC:.c=.o)

.PHONY: clean cclean zip

all: tail tail2 wordcount wordcount-dynamic

tail: tail.c
	$(CC) $(CFLAGS) $< -o $@

tail2: tail2.cc
	$(CXX) $(CXXFLAGS) $< -o $@

wordcount: wordcount.o $(SNAME) io.o
	$(CC) $(CFLAGS) -Bstatic -o wordcount $^ 

wordcount-dynamic: io.o $(DNAME) wordcount.o
	$(CC) $(CFLAGS) -o wordcount-dynamic $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -fPIC

# knihovny:
$(SNAME): $(HTAB_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(DNAME): $(HTAB_OBJ)
	$(CC) $(CFLAGS) -shared $(HTAB_OBJ) -o libhtable.so

clean: 
	rm -rf *.o *.dSYM

cclean:
	rm -rf *.o $(SNAME) $(DNAME) tail tail2 wordcount wordcount-dynamic *.dSYM xpiste04.zip

zip:
	zip xpiste04.zip *.c *.cc *.h Makefile

# End of file: Makefile 

