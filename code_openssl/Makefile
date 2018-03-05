
CC=gcc
OBJ = opensslserver.o
opensslserver: opensslserver.o
	$(CC) -std=c99 -pthread -Wall -o opensslserver /usr/lib opensslserver.c -lssl -lcrypto
