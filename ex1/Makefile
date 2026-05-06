CC = gcc
CFLAGS = -Wall -Wextra -O2

all: escritor leitor

escritor: escritor.c
	$(CC) $(CFLAGS) -o escritor escritor.c

leitor: leitor.c
	$(CC) $(CFLAGS) -o leitor leitor.c

clean:
	rm -f escritor leitor /tmp/ex1_fifo