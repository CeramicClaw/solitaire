CC = gcc
CFLAGS = -ggdb3 -Wall -Werror -pedantic
DEPS = deck.o main.o
PROJECT = solitaire

${PROJECT} : $(DEPS)
	$(CC) -o ${PROJECT} $(DEPS)
deck.o:
	$(CC) $(CFLAGS) -c src/deck.c
main.o:
	$(CC) $(CFLAGS) -c src/main.c
clean:
	rm -rf *.o
cleanall:
	rm -rf ${PROJECT} *.o
