CC=gcc
CFLAGS=-Wall -Wextra -Wshadow

all: clean arbitro grusia

arbitro: arbitro.c
	$(CC) $(CFLAGS) -o arbitro.exe arbitro.c

grusia: grusia.c
	$(CC) $(CFLAGS) -o grusia.exe grusia.c

clean:
	rm -f arbitro.exe grusia.exe
