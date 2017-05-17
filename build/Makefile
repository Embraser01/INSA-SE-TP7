CFLAGS = -g -I../include/ -std=c99 -Wall -O3 -pthread
CC=gcc
OBJS =  obj/edge_detect.o \
	    obj/pgm.o \
	    obj/deriche.o 

INCS = ../include/common.h
PROG = deriche
SRCDIR = ../src/

all: $(PROG)


deriche : $(OBJS) $(INCS) 
	$(CC) $(CFLAGS)  -o $(PROG) $(OBJS) -lm

obj/edge_detect.o : $(SRCDIR)edge_detect.c $(INCS) 
	$(CC) $(CFLAGS) -c -o obj/edge_detect.o $(SRCDIR)edge_detect.c

obj/pgm.o : $(SRCDIR)pgm.c $(INCS)
	$(CC) $(CFLAGS) -c -o obj/pgm.o $(SRCDIR)pgm.c

obj/deriche.o : $(SRCDIR)deriche.c $(INCS)
	$(CC) $(CFLAGS)  -c -o obj/deriche.o $(SRCDIR)deriche.c 



clean:
	rm -f obj/* $(PROG)
