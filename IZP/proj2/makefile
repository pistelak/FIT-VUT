NAME=proj2
OBJFILES=$(NAME).o proj2_log.o proj2_arctg.o proj2_powxa.o proj2_argsinh.o proj2_exp.o 

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -W -g -O3 -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all: $(NAME)

clean:
	rm -f *.o 
dep: 
	$(CC) -MM *.c >dep.list

-include dep.list

proj2.o: proj2.c proj2_arctg.h proj2_powxa.h proj2_argsinh.h
proj2_arctg.o: proj2_arctg.c
proj2_argsinh.o: proj2_argsinh.c proj2_log.h
proj2_exp.o: proj2_exp.c
proj2_log.o: proj2_log.c
proj2_powxa.o: proj2_powxa.c proj2_log.h proj2_exp.h


$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@
