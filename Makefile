all: main

main: main.o cpu.o utils.o menu.o
	gcc -o main main.o cpu.o utils.o menu.o -lSDL3 -lSDL3_ttf

main.o: src/main.c
	gcc -c src/main.c -o main.o

cpu.o: src/cpu.c
	gcc -c src/cpu.c -o cpu.o

utils.o: src/utils.c
	gcc -c src/utils.c -o utils.o

menu.o: src/menu.c
	gcc -c src/menu.c -o menu.o

clean:
	rm -f main main.o cpu.o utils.o menu.o