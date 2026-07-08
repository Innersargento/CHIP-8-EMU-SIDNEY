all: main

main: main.o cpu.o utils.o
	gcc -o main main.o cpu.o utils.o

main.o: main.c
	gcc -c main.c -o main.o

cpu.o: cpu.c
	gcc -c cpu.c -o cpu.o

utils.o: utils.c
	gcc -c utils.c -o utils.o

clean:
	rm -f main main.o cpu.o