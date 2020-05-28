#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

// Esta es la estructura principal de la lista enlazada 
// Se define los nodos de la lista enlazada
// tiene dos partes:
//      1) El valor que es una cadena de caracteres
//      2) Un apuntador al siguiente nodo de la lista 
typedef struct{
    char valor[512];
    struct lista_enlazada *lista_segundo_nivel;
    struct nodo *siguiente;
}nodo;

// Estructura de la lista enlazada 
typedef struct{
    nodo* cabeza_lista;
    int tamano_lista;
} lista_enlazada;

// Funcion para crear nodos 
// El segundo argumento significa si es una lista de lista
// Si es 0 entonces es una lista de primer nivel 
// si es 1 es una lista de listas.
nodo *inicializar_nodo(char *str, int tipo_lista);

// Inicializar la lista enlazada 
lista_enlazada* inicializar_lista();

// Funcion para insertar elementos para la lista,
void insertar_lista(lista_enlazada *lista, char *str, char path[]);

void insertar_lista_segundo_nivel(lista_enlazada *lista, char *str, char path[]);

// Eliminar elementos en la lista. 
int eliminar_lista(lista_enlazada *lista, char *valor);

// Buscar elemento en la lista 
int buscar_lista(lista_enlazada *lista, char *valor);

// Retorna el tamano de la lista 
int size_lista(lista_enlazada *lista);

// Mostrar lista 
void print_lista(lista_enlazada *lista);

#endif 