CFLAGS=-c -Wall

all: main.o
	gcc main.o -o life -lncurses

main.o: main.c 
	gcc $(CFLAGS) main.c

clean:
	rm -rf *~ life *.o *.txt
