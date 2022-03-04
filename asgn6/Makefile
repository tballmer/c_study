CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt

keygen: keygen.o randstate.o numtheory.o rsa.o
	$(CC) -o keygen keygen.o randstate.o numtheory.o rsa.o $(LFLAGS)

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

encrypt: encrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o encrypt encrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: decrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o decrypt decrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c

numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c

rsa.o: rsa.c
	$(CC) $(CFLAGS) -c rsa.c

format:
	clang-format -i -style=file *.[c,h]

clean:
	rm -f keygen encrypt decrypt *.o

debug: CFLAGS += -g
debug: all
