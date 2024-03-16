CC = gcc
CFLAGS = -Wall -c -fPIC
OBJECTS = obj/vec.o obj/matrix.o obj/nodeprimitive.o # obj/ss1dcircuits.o

build: $(OBJECTS)
	$(CC) -Wall -shared $(OBJECTS) -o lib/libcneapolitan.so

obj/vec.o:
	$(CC) $(CFLAGS) src/vec.c -o obj/vec.o

obj/matrix.o:
	$(CC) $(CFLAGS) src/matrix.c -o obj/matrix.o

obj/nodeprimitive.o:
	$(CC) $(CFLAGS) src/nodeprimitive.c -o obj/nodeprimitive.o

# obj/ss1dcircuits.o:
# 	$(CC) $(CFLAGS) src/ss1dcircuits.c -o obj/ss1dcircuits.o