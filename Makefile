CC = gcc
CFLAGS = -Wall -Werror -pedantic
DEPS = cards.o deck.o main.o
PROJECT = solitaire

${PROJECT} : $(DEPS)
	$(CC) -o ${PROJECT} $(DEPS)
cards.o:
	$(CC) $(CFLAGS) -c cards.c cards.h
deck.o:
	$(CC) $(CFLAGS) -c deck.c deck.h
main.o:
	$(CC) $(CFLAGS) -c main.c deck.h
clean:
	rm -rf *.o *.gch
