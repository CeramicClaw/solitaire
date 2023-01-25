CC = gcc
CFLAGS = -Wall -Werror -pedantic
DEPS = cards.o deck.o main.o
PROJECT = solitaire

${PROJECT} : $(DEPS)
	$(CC) -o ${PROJECT} $(DEPS)
cards.o:
	$(CC) $(CFLAGS) -c src/cards.c
deck.o:
	$(CC) $(CFLAGS) -c src/deck.c
main.o:
	$(CC) $(CFLAGS) -c src/main.c
clean:
	rm -rf *.o
cleanall:
	rm -rf ${PROJECT} *.o
