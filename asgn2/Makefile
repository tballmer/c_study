CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LFLAGS = -lm

all: mathlib-test

mathlib-test: mathlib-test.o e.o newton.o madhava.o euler.o bbp.o viete.o
	$(CC) -o mathlib-test mathlib-test.o e.o newton.o madhava.o euler.o bbp.o viete.o $(LFLAGS) 

mathlib-test.o: mathlib-test.c mathlib.h
	$(CC) $(CFLAGS) -c mathlib-test.c

e.o: e.c
	$(CC) $(CFLAGS) -c e.c

newton.o: newton.c
	$(CC) $(CFLAGS) -c newton.c

madhava.o: madhava.c
	$(CC) $(CFLAGS) -c madhava.c

euler.o:
	$(CC) $(CFLAGS) -c euler.c

bbp.o:
	$(CC) $(CFLAGS) -c bbp.c

viete.o:
	$(CC) $(CFLAGS) -c viete.c

format:
	clang-format -i -style=file *.[c,h]

clean:
	rm -f mathlib-test *.o
