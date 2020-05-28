#include "tabla.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_enlazada.h"

#define TAMANO_TABLA 15


// Definicion de la estructura a utilizar.
// struct pair{
// 	char value;
// 	int key;	
// };

pair hash_table[TAMANO_TABLA];


// Se inicializa la tabla con todos los valores en -1.
void inicializar_tabla(){
	int i = 0;
	while(i < TAMANO_TABLA){
		lista_enlazada *auxiliar = inicializar_lista();
		hash_table[i].lista = auxiliar;
		hash_table[i].key = 0;
		i++;
	}
	// printf("Se inicializo exitosamente la tabla de hash\n");
}


// Funcion para buscar un elemento en la tabla de hash de regresa su clave asociada.
// int search(pair hash_table[], int TAMANO_TABLA, char elemento[]){
// 	int s;
// 	s = ascii(elemento);
// 	return buscar_lista(hash_table[s % TAMANO_TABLA].lista, s);
// };


// Funcion para ingresar un nuevo elemento a la tabla.
void insert(char elemento[], char path[]){
	int inp;
	// char auxiliar_path[1024];
	// strcpy(auxiliar_path, path);
	strcat(path, "\n");

	inp = ascii(elemento);
	inp = inp % TAMANO_TABLA;
	insertar_lista(hash_table[inp].lista, elemento, path);
	// printf("Se inserto exitosamente en la tabla hash\n");
	/*
	else{

	}*/
};


// Funcion para buscar en la tabla de hash.
int search(char elemento[]){
	int inp;
	inp = ascii(elemento);
	inp = inp % TAMANO_TABLA;
	// printf("%s, %d\n", elemento, inp);	
	if (buscar_lista(hash_table[inp].lista, elemento) == 1){
		// printf("Se encontro el elemento\n");
		nodo *iterador = hash_table[inp].lista -> cabeza_lista;
		while(iterador != NULL){
			if (strcmp(elemento, iterador -> valor) == 0){
				print_lista(iterador -> lista_segundo_nivel);
				break;
			}
			iterador = iterador -> siguiente;
		}
		return 1;
	}
	// printf("El elemento no se encuentra\n");
}


void print_hash_table(){
	int iterador;
	for (iterador = 0; iterador < TAMANO_TABLA; iterador++){
		printf("%d: ", iterador);
		print_lista(hash_table[iterador].lista);
		printf("\n");
	}
}

// Funcion que transforma un string en su representacion de ascii.
int ascii(char palabra[]){
	int i,suma = 0;
	for (i = 0; palabra[i] != '\0'; i++){
		suma = suma + palabra[i];
	}
	return suma;
}


// Prueba de la tabla de hash 
// int main(){
// 	inicializar_tabla();
// 	insert("a");
// 	insert("b");
// 	insert("c");
// 	insert("d");
// 	insert("e");
// 	insert("f");
// 	print_hash_table();
// 	search("a");
// 	search("b");
// 	search("c");
// 	search("d");
// 	search("e");
// 	search("f");



// 	return 0;
// }