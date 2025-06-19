CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors

symnmf: symnmf.o
	$(CC) $(CFLAGS) -o symnmf symnmf.o -lm

symnmf.o: symnmf.c symnmf.h
	$(CC) $(CFLAGS) -c symnmf.c -o symnmf.o

clean:
	rm -f *.o symnmf
