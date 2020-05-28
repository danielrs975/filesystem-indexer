CC=gcc -std=gnu99 -g -pthread -Wall
CFLAGS=-c
OBJECTS=tabla.o lista_enlazada.o

main_proyecto: 
	gcc -g main_proyecto.c tabla.c lista_enlazada.c -o main_proyecto -lpthread

clean:
	-rm main_proyecto
	-rm *.o