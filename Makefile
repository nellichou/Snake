#################################
# SNAKE Makefile
#################################
GCC=gcc -o 
CFLAGS=-ansi -pedantic -lgraph
run: snake
	./snake
snake: snake.o fonctions.o 
	$(GCC) snake snake.o fonctions.o $(CFLAGS)
fonctions.o: fonctions.c fonctions.h
	$(GCC) fonctions.o -c fonctions.c $(CFLAGS)
snake.o: snake.c fonctions.h
	$(GCC) snake.o -c snake.c $(CFLAGS)
clean:
	rm -f *.o snake
# END MAKEFILE ####################
