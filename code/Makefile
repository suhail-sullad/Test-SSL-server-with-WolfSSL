
CC=gcc
CFLAGS=-Wall -I ../include
DEPS = ../include/unp.h
OBJ = wolfsslserver.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wolfsslserver: ../lib/*.c wolflib.c
	$(CC) -std=c99 -pthread -Wall -o wolfsslserver ../lib/*.c wolflib.c -I ../include -lm -lwolfssl
