CC = gcc
CFLAGS = -Wall -c -fPIC
OBJECTS = obj/vec.o obj/matrix.o

build: $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o lib/libcneapolitan.so

obj/vec.o:
	$(CC) $(CFLAGS) src/vec.c -o obj/vec.o

obj/matrix.o:
	$(CC) $(CFLAGS) src/matrix.c -o obj/matrix.o