CC=gcc
# cflags=-O2 -std=c11 -Wshadow -Wall -pedantic -lm
cflags=-g -ggdb -std=c11 -Wshadow -Wall -pedantic -lm
dbg=-fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -g -DLOCAL
glut=-lGL -lGLU -lglut
objs=obj/linear_algebra.o obj/colour.o obj/drawing_routines.o \
	 obj/keyboard.o obj/texture.o obj/level_generator.o obj/enums.o

bin/game.out: src/main.c $(objs)
	$(CC) $(cflags) $(objs) src/main.c -o bin/game.out $(glut)

obj/linear_algebra.o: src/linear_algebra.c include/linear_algebra.h
	$(CC) $(cflags) -c src/linear_algebra.c -o obj/linear_algebra.o

obj/colour.o: src/colour.c include/colour.h
	$(CC) $(cflags) -c src/colour.c -o obj/colour.o

obj/drawing_routines.o: src/drawing_routines.c include/drawing_routines.h
	$(CC) $(cflags) -c src/drawing_routines.c -o obj/drawing_routines.o

obj/keyboard.o: src/keyboard.c include/keyboard.h
	$(CC) $(cflags) -c src/keyboard.c -o obj/keyboard.o

obj/texture.o: src/texture.c include/texture.h
	$(CC) $(cflags) -c src/texture.c -o obj/texture.o

obj/level_generator.o: src/level_generator.c include/level_generator.h
	$(CC) $(cflags) -c src/level_generator.c -o obj/level_generator.o

obj/enums.o: src/enums.c include/enums.h
	$(CC) $(cflags) -c src/enums.c -o obj/enums.o

run: bin/game.out
	./bin/game.out

prepare:
	mkdir -p obj
	mkdir -p bin
