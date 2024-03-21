CC = gcc
CFLAGS = -Wall -c -fPIC
TEST_CFLAGS = -Wall -c -I src

LIBPATH = build/lib/libneapolitan.so
OBJPATH = build/obj
BINPATH = build/bin

OBJECTS = build/obj/vec.o build/obj/matrix.o build/obj/elemprimitives.o

# =============
# Build recipes
# =============
build: clean $(OBJECTS)
	$(CC) -Wall -shared $(OBJECTS) -o $(LIBPATH)

build/obj/vec.o:
	$(CC) $(CFLAGS) src/vec.c -o $(OBJPATH)/vec.o

build/obj/matrix.o: build/obj/vec.o
	$(CC) $(CFLAGS) src/matrix.c -o $(OBJPATH)/matrix.o

build/obj/elemprimitives.o: build/obj/vec.o
	$(CC) $(CFLAGS) src/elemprimitives.c -o $(OBJPATH)/elemprimitives.o

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
	./$(BINPATH)/vectests
	
matrixtests: build
	$(CC) $(TEST_CFLAGS) test/matrixtests.c -o $(OBJPATH)/matrixtests.o
	$(CC) -Wall $(OBJPATH)/matrixtests.o $(LIBPATH) -o $(BINPATH)/matrixtests
	./$(BINPATH)/matrixtests