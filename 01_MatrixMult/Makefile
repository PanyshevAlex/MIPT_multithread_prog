all: matrixmult

matrixmult: main.o matrixmult.o
	gcc main.o solution.o -o matrixmult

main.o:
	gcc -lpthread -c main.c -o main.o

matrixmult.o:
	gcc -lpthread -c solution.c -o solution.o

clean:
	rm -rf *.o matrixmult