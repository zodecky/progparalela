
FLAGS = 

run: trabalho2
	./trabalho2 $(FLAGS)

trabalho2: main.o getargs.o
	gcc -o trabalho2 main.o getargs.o -Wincompatible-pointer-types

main.o: main.c
	gcc -c main.c

getargs.o: getargs.c
	gcc -c getargs.c

clean:
	rm *.o trabalho2