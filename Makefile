CC = gcc
CFLAGS = -ggdb3 -Wall -Werror -pedantic
DEPS = deck.o file.o cipher.o main.o
PROJECT = solitaire

${PROJECT} : $(DEPS)
	$(CC) -o ${PROJECT} $(DEPS)
deck.o: src/deck.c src/deck.h
		$(CC) $(CFLAGS) -c src/deck.c
file.o: src/file.c src/file.h
		$(CC) $(CFLAGS) -c src/file.c
cipher.o: src/cipher.c src/cipher.h
	$(CC) $(CFLAGS) -c src/cipher.c
main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c
clean:
	rm -rf *.o
cleanall:
	rm -rf ${PROJECT} *.o
