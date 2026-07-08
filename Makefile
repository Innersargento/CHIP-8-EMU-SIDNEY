all: main

main: main.o cpu.o utils.o
	gcc -o main main.o cpu.o utils.o

main.o: src/main.c
	gcc -c src/main.c -o main.o

cpu.o: src/cpu.c
	gcc -c src/cpu.c -o cpu.o

utils.o: src/utils.c
	gcc -c src/utils.c -o utils.o

clean:
	rm -f main main.o cpu.o