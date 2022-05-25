CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -lpthread

all: clean sesiones pergaminos

sesiones: sesiones.c
	$(CC) $(CFLAGS) -o sesiones.exe sesiones.c

pergaminos: pergaminos.c
	$(CC) $(CFLAGS) -o pergaminos.exe pergaminos.c

clean:
	rm -f sesiones.exe pergaminos.exe
