CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -lpthread

all: clean sesiones

sesiones: sesiones.c
	$(CC) $(CFLAGS) -o sesiones.exe sesiones.c

clean:
	rm -f sesiones.exe
