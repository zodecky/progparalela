trabalho1: trabalho1.c parser.c escalona.c
	gcc -o trabalho1 trabalho1.c parser.c escalona.c -lpthread

run: trabalho1 exec.txt prog1.c prog2.c prog3.c prog4.c
	gcc prog1.c -o prog1; gcc prog2.c -o prog2; gcc prog3.c -o prog3; gcc prog4.c -o prog4; gcc prog5.c -o prog5; ./trabalho1

clean:
	rm -f *.o trabalho1