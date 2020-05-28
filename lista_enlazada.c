#include "lista_enlazada.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Creacion de los nodos 
nodo* inicializar_nodo(char *str, int tipo_lista){
    nodo* aux = malloc(sizeof(nodo));
    strcpy(aux -> valor, str);
    aux -> siguiente = NULL;
    if (tipo_lista == 1){
        lista_enlazada *auxiliar = inicializar_lista();
        aux -> lista_segundo_nivel = auxiliar;
    }
    return aux;
}

// Reservacion del espacio para la creacion de la lista.
lista_enlazada* inicializar_lista(){
    lista_enlazada *auxiliar = malloc(sizeof(lista_enlazada));
    auxiliar -> cabeza_lista = NULL;
    auxiliar -> tamano_lista = 0;
    // printf("Se inicializo la lista exitosamente\n");
    return auxiliar;
}

// Insertar un nodo en la lista 
void insertar_lista(lista_enlazada *lista, char *str, char path[]){
    
    // Primero busco si el elemento esta en la lista 
    if (buscar_lista(lista, str) == 0){
        // Si el elemento esta en la lista entonces inicializo el nodo con 
        // la cadena de caracteres pasada, se genera el nodo 
        // y se agrega a la lista
        nodo *aux = inicializar_nodo(str, 1);

        if (lista -> cabeza_lista == NULL){
            // IMPORTANTE: Verifico si la lista esta vacia
            lista -> cabeza_lista = aux;
        }
        else{
            aux -> siguiente = lista -> cabeza_lista;
            lista -> cabeza_lista = aux;
        }

        (lista -> tamano_lista)++;
        insertar_lista_segundo_nivel(lista, str, path);
        // printf("Elemento agregado exitosamente\n");
    }
    else{
        // Si ya el termino esta en la tabla de hash pero el termino pertenece
        // en un path diferente entonces se agrega en la lista del segundo nivel.
        insertar_lista_segundo_nivel(lista, str, path);
    }
}

void insertar_lista_segundo_nivel(lista_enlazada* lista, char *str, char path[]){
    if (lista -> tamano_lista > 0){
        nodo *iterador = lista -> cabeza_lista;
        while (iterador != NULL){
            if (strcmp(iterador -> valor, str) == 0){
                nodo *aux = inicializar_nodo(path, 0);
                lista_enlazada *lista_de_lista = iterador -> lista_segundo_nivel;
                if ( lista_de_lista -> cabeza_lista == NULL){
                    lista_de_lista -> cabeza_lista = aux;
                }
                else {
                    aux -> siguiente = lista_de_lista -> cabeza_lista;
                    lista_de_lista -> cabeza_lista = aux;
                }
                (lista_de_lista -> tamano_lista)++;
                // printf("Elemento agregado exitosamente en la lista de segundo nivel\n");
                break;
            }
            iterador = iterador -> siguiente;
        }
    }
}


// Buscar un nodo en la lista
int buscar_lista(lista_enlazada *lista, char *str){
    if (lista -> tamano_lista > 0){
        nodo* iterador = lista -> cabeza_lista;
        while (iterador != NULL){
            if (strcmp(iterador -> valor, str) == 0){
                return 1;
            }
            iterador = iterador -> siguiente;
        }
    }
    return 0;
}

int size_lista(lista_enlazada *lista){
    return lista -> tamano_lista;
}

// Funcion que muestra la lista.
void print_lista(lista_enlazada *lista){
    nodo* iterador = lista -> cabeza_lista;
    while(iterador != NULL){
        printf("%s", iterador -> valor);
        iterador = iterador -> siguiente;
    }
    printf("\n");
}