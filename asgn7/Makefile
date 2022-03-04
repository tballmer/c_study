CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LFLAGS = -lm

all: banhammer

banhammer: banhammer.o bv.o bf.o speck.o node.o bst.o ht.o pq.o parser.o
	$(CC) -o banhammer banhammer.o bv.o bf.o speck.o node.o bst.o ht.o pq.o parser.o $(LFLAGS)

banhammer_test.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c	

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c	

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c	

node.o: node.c
	$(CC) $(CFLAGS) -c node.c	

bst.o: bst.c
	$(CC) $(CFLAGS) -c bst.c	

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c	

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c	

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c	

format:
	clang-format -i -style=file *.[c,h]

clean:
	rm -f banhammer *.o

debug: CFLAGS += -g
debug: all
