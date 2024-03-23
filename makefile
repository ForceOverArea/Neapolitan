CC = gcc
CFLAGS = -Wall -c -fPIC
TEST_CFLAGS = -Wall -c -I src

LIBPATH = build/lib/libneapolitan.so
OBJPATH = build/obj
BINPATH = build/bin

OBJECTS = build/obj/vec.o build/obj/matrix.o build/obj/elems.o

# =============
# Build recipes
# =============
build: clean $(OBJECTS)
	$(CC) -Wall -shared $(OBJECTS) -o $(LIBPATH)

build/obj/vec.o:
	$(CC) $(CFLAGS) src/vec.c -o $(OBJPATH)/vec.o

build/obj/matrix.o: build/obj/vec.o
	$(CC) $(CFLAGS) src/matrix.c -o $(OBJPATH)/matrix.o

build/obj/elems.o: build/obj/vec.o
	$(CC) $(CFLAGS) src/elems.c -o $(OBJPATH)/elems.o

# =====
# Clean
# =====
delete:
	rm -r build

regen:
	mkdir build/bin -p
	mkdir build/lib
	mkdir build/obj

clean: delete regen

# =====
# Tests
# =====
vectests: build
	$(CC) $(TEST_CFLAGS) test/vectests.c -o $(OBJPATH)/vectests.o
	$(CC) -Wall $(OBJPATH)/vectests.o $(LIBPATH) -o $(BINPATH)/vectests
	clear
	./$(BINPATH)/vectests
	
matrixtests: build
	$(CC) $(TEST_CFLAGS) test/matrixtests.c -o $(OBJPATH)/matrixtests.o
	$(CC) -Wall $(OBJPATH)/matrixtests.o $(LIBPATH) -o $(BINPATH)/matrixtests
	clear
	./$(BINPATH)/matrixtests

elemstests: build
	$(CC) $(TEST_CFLAGS) test/elemstests.c -o $(OBJPATH)/elemstests.o
	$(CC) -Wall $(OBJPATH)/elemstests.o $(LIBPATH) -o $(BINPATH)/elemstests
	clear
	./$(BINPATH)/elemstests