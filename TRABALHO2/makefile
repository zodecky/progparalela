
FLAGS = 
CFLAGS = -g

run: trabalho2
	./trabalho2 $(FLAGS)

trabalho2: main.o getargs.o pages.o
	gcc -o trabalho2 main.o getargs.o pages.o -Wincompatible-pointer-types $(CFLAGS) -lm

main.o: main.c
	gcc -c main.c $(CFLAGS)

getargs.o: getargs.c
	gcc -c getargs.c $(CFLAGS)

pages.o: pages.c
	gcc -c pages.c $(CFLAGS)

clean:
	rm *.o trabalho2