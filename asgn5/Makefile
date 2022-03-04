CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LFLAGS = -lm

all: encode decode

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o encode encode.o node.o pq.o code.o io.o stack.o huffman.o $(LFLAGS)

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode: decode.o io.o huffman.o stack.o node.o pq.o code.o
	$(CC) -o decode decode.o io.o huffman.o stack.o node.o pq.o code.o $(LFLAGS)

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c	

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

format:
	clang-format -i -style=file *.[c,h]

clean:
	rm -f encode decode *.o

debug: CFLAGS += -g
debug: all
	
