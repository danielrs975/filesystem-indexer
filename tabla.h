#ifndef TABLA_HASH_H
#define TABLA_HASH_H
#include "lista_enlazada.h"

// Definicion de la estructura a utilizar.
typedef struct {
	lista_enlazada *lista;
	int key;
} pair;

// Inicializacion de la tabla de hash
void inicializar_tabla();

// Funcion que busca si exite un elemento en la tabla, 
// retorna 1 en caso de que exista y 0 en caso contrario.
int search(char elemento[]);

// Funcion para insertar elementos en la tabla.
void insert(char elemento[], char path[]);

// Funcion que devuelve la suma de la representacion ASCII de un string.
int ascii(char palabra[]);

// Imprimir tabla de hash.
void print_hash_table();

#endif