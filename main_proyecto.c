#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "tabla.h"
#include <pthread.h>

// Creacion del mutex 
int mutex = 0;

char archivo_indice[1600];

typedef struct stat i_node;
// IMPORTANTE:
//      1) Si se colocan flags en la entrada del programa el termino de la busqueda va despues de todos los
//      flags
//      2) Formato del indice primero se pone el path y luego la descomposicion de el nombre del archivo por el caracter
//      de espacio

// Opciones del programa. 0 Si esta desactivado y 1 si esta activado 

// Primera opcion. Directorio donde se comienza la busqueda 
int opcion_d = 0;
// Por default se empieza la busqueda en la carpeta actual.
char carpeta_inicio[256] = "./";

char *flag_d = "-d";
char *flag_opcional_d = "--dir";

// Segunda opcion. Numero de niveles que de subdirectorios que se exploraran 
// para la actualizacion del indice.
int opcion_m = 0;
// Por default la altura maxima es de 20 
int altura_busqueda = 20;

char *flag_m = "-m";
char *flag_opcional_m = "--max";

// Tercera opcion. Archivo que contiene el indizador, es decir, 
// la tabla de indice que contiene la informacion de los nombres de los 
// archivos
int opcion_i = 0;
char tabla_indice[256] = "indice_salida.txt";

char *flag_i = "-i";
char *flag_opcional_i = "index";
// Cuarta opcion. Indica que no se debe entrar en directorios que ya esten 
// en el indice. Solo en los directorios que en encuentre nuevos.
int opcion_u = 0;

char *flag_u = "-u";
char *flag_opcional_u = "--noupdate";
// Quinta opcion. Si esta opcion esta activa entonces no se deben entrar en directorios 
// que no esten en el indice.
int opcion_a = 0;

char *flag_a = "-a";
char *flag_opcional_a = "--noadd";

// Si la opcion es -ua no se recorren los directorios; solo se consulta el indice.

char *flag_ua = "-ua";

// Entrada del termino que se va a buscar
char termino_a_buscar[256];



// Rutina para la actualizacion del indice. 
void escribir_archivo(char salida[], char path[], char archivo[]){

    char *terminos = strtok(archivo, " ");
    char conexion[1600] = "-";
    strcat(archivo_indice, path);
    while (terminos != NULL){
        strcat(conexion, terminos);
        strcat(archivo_indice, conexion);
        terminos = strtok(NULL, " ");
        conexion[0] = '-';
        conexion[1] ='\0';
    }
    strcat(archivo_indice, "\n");
}

// Funciones para las opciones.

int ejecutar_opcion(char *flag){
    // Los distintos flags que se presentan en este proyecto 
    if (strcmp(flag, flag_d) == 0|| strcmp(flag, flag_opcional_d) == 0){
        // printf("Se activo la opcion d\n");
        opcion_d = 1;
        return 1;
    }
    else if (strcmp(flag, flag_m) == 0 || strcmp(flag, flag_opcional_m) == 0){
        // printf("Se activo la opcion m\n");
        opcion_m = 1;
        return 2;
    }
    else if (strcmp(flag, flag_i) == 0 || strcmp(flag, flag_opcional_i) == 0){
        // printf("Se activo la opcion i\n");
        opcion_i = 1;
        return 3;
    }
    else if (strcmp(flag, flag_u) == 0 || strcmp(flag, flag_opcional_u) == 0){
        // printf("Se activo la opcion u\n");
        opcion_u = 1;
    }
    else if (strcmp(flag, flag_a) == 0 || strcmp(flag, flag_opcional_a) == 0){
        // printf("Se activo la opcion a\n");
    }
    else if (strcmp(flag, flag_ua) == 0){
        // printf("Se activo la opcion ua\n");
        opcion_u = 1;
        opcion_a = 1;
    }
    else {
        // printf("Se introdujo un flag invalido. Saliendo del programa. O un archivo\n");        
        // exit(0);
        return -1;
    }
    return 0;
}

// Comprobar si lo siguiente a un flag con argumento es un argumento.

int es_argumento(int argumento){
    if (argumento == 1 || argumento == 2 || argumento == 3 || argumento == 0){
        // printf("Esta entrada no es valida. Saliendo del programa\n");
        exit(0);
    }
    return 1;
}

// Funciones para la busqueda en los directorios.

int is_dir(const char* path) {
    i_node buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

// Hilo para la busqueda dentro del directorio, 
// Recorrido del arbol de directorios 
void dfs(char *path_inicio, int altura){
    // Declaro las estructuras necesarias para poder recorrer
    // el arbol de directorios

    struct dirent *directorio;
    char copia_path[1024];

    // Contador de numero de archivos dentro de un directorio
    int numero_archivos = 0;

    strcpy(copia_path, path_inicio);
    
    // Si el arbol alcanzo la altura maxima entonces corta el dfs en ese punto
    if (altura + 1 > altura_busqueda){
        return;
    }

    // Abro el directorio
    DIR *directorio_actual = opendir(path_inicio);
    // Recorro el contenido de tal directorio.
    while ((directorio = readdir(directorio_actual)) != NULL){
        // Verifico si el archivo actual es un archivo o un directorio
        // Los directorios que no se van a leer son . y ..
        if (strcmp(directorio -> d_name, ".") == 0 || strcmp(directorio ->d_name, "..") == 0){
            continue;
        }
        // printf("%s\n", strcat(copia_path, directorio -> d_name));
        strcat(copia_path, "/");
        if (is_dir(strcat(copia_path, directorio -> d_name))){
            // Si es un directorio entonces vuelvo a hacer dfs sobre esa
            // direccion
            
            int copia_altura = altura;
            
            dfs(copia_path, ++copia_altura);
        }
        else{
            // Busca si el termino de busqueda pertenece al nombre del archivo             
            char *lo_encontro;
            lo_encontro = strstr(directorio -> d_name, termino_a_buscar);
            // strcat(copia_path, directorio -> d_name);
            
            if (lo_encontro != NULL){
                // Estrategia de mutex 
                while(mutex == 1){
                    continue;
                }

                // Escribir en un archivo de salida la actualizacion 
                // del indice.

                escribir_archivo(tabla_indice, copia_path, directorio -> d_name);
                printf("%s\n", copia_path);
                insert(termino_a_buscar, copia_path);
            }
            else{
                // printf("El archivo no contiene tal termino.\n");
            }
            // printf("El archivo: %s\n", directorio -> d_name);
            // printf("Es un archivo\n");
            // printf("Palindromo: %s\n", copia_cadena);
        }
    
        copia_path[0] = '\0';
        strcpy(copia_path, path_inicio);
        numero_archivos++;
    }
    // if (numero_archivos == 0){
        // printf("Palindromo: %s\n", cadena);
    // }
}



// Ejecucion de dfs con hilos
void *dfs_atraves_hilo(void* vargp){
    dfs(carpeta_inicio, 0);
}

// Funciones intermediarias
void buscar_termino_hash(char termino[]){
    mutex = 1;
    search(termino);
    mutex = 0;
}

void cargar_indice(char * archivo){
    char caracter;
    char str[1000];
    char path[1000];
    int contador_espacios = 0;
    int iterador = 0;
    FILE* entrada = fopen(archivo, "r");
    if (entrada == NULL){
        perror("Error en la apertura del archivo\n");
        exit(0);
    }

    while(feof(entrada) == 0){
        caracter = fgetc(entrada);

        if (caracter == '-'){
            if (contador_espacios == 0){
                strcpy(path, str);
                path[iterador] = '\0';
            }
            else {
                insert(str, path);
            }
            str[0] = '\0';
            iterador = 0;
            contador_espacios++;
            continue;
        }
        else if(caracter == '\n'){
            str[iterador] = '\0';
            insert(str, path);
            str[0] = '\0';
            contador_espacios = 0;
            iterador = 0;
            path[0] = '\0';
            continue;
        }
        str[iterador] = caracter;
        iterador++;
    }
    fclose(entrada);
}

// Main del proyecto de operativos II
// Aqui es donde se van a manejar todo las estructuras del proyecto 

int main(int argc, char *argv[]){
    // Inicializacion de la tabla de hash.
    inicializar_tabla();
    
    // Veo si el numero de argummentos introducidos es mayor que 2, es decir, 
    // que el introdujeron otros argumentos a parte del nombre del programa 
    // y el termino de busqueda 
    int iterador_opciones;
    
    // Esta variable va a almacenar un numero que indica si el flag va junto con un
    // argumento o no 
    // Si retorna 1, 2, 3 entonces es un flag que viene acompanado con un argumento;
    // Si retorna 0 entonces es un flag que no viene con un argumento 
    // Si retorna -1 es un un flag que no existe o es un argumento

    int flag_con_argumento;
    int si_es_argumento;
    int es_argumento_valido;
    char opcion[256];



    if (argc > 2){
        // Recorro todas las opciones que se han puesto en la terminal;
        for (iterador_opciones = 1; iterador_opciones < argc - 1; iterador_opciones++){
            strcpy(opcion, argv[iterador_opciones]);
            flag_con_argumento = ejecutar_opcion(opcion);

            // Vemos si el argumento que va al lado del flag es un argumento.
            if (flag_con_argumento == 1){
                si_es_argumento = ejecutar_opcion(argv[iterador_opciones + 1]);
                
                es_argumento_valido = es_argumento(si_es_argumento);
                if (es_argumento_valido == 1){
                    carpeta_inicio[0] = '\0';
                    strcpy(carpeta_inicio, argv[iterador_opciones + 1]);
                }
            }
            else if (flag_con_argumento == 2){
                si_es_argumento = ejecutar_opcion(argv[iterador_opciones + 1]);
                
                es_argumento_valido = es_argumento(si_es_argumento);
                if (es_argumento_valido == 1){
                    altura_busqueda = atoi(argv[iterador_opciones + 1]);
                }
            }
            else if (flag_con_argumento == 3){
                si_es_argumento = ejecutar_opcion(argv[iterador_opciones + 1]);
                
                es_argumento_valido = es_argumento(si_es_argumento);
                if (es_argumento_valido == 1){
                    strcpy(tabla_indice, argv[iterador_opciones + 1]);
                    cargar_indice(tabla_indice);
                }
            }
        }
        strcpy(termino_a_buscar, argv[argc - 1]);
    }
    else {
        // Tomar el termino de busqueda
        flag_con_argumento = ejecutar_opcion(argv[1]);
        if (flag_con_argumento != -1){
            // printf("La entrada no es la correcta. Saliendo del programa\n");
            exit(0);
        }
        strcpy(termino_a_buscar, argv[1]);
    }


    // Busqueda en el directorio 
    // dfs(carpeta_inicio, 0);

    // Busqueda en el indice.
    if (opcion_a == 1 && opcion_u == 1){
        search(termino_a_buscar);        
    }
    else{
        // Creacion de hilos. 
        // Hilo que va a ejecutar dfs. Es el que se encarga de buscar en el arbol de directorios
        pthread_t hilo_dfs;
        int i = 2;
        // buscar_termino_hash(termino_a_buscar);

        pthread_create(&hilo_dfs, NULL, dfs_atraves_hilo, NULL);
        // Antes que termine el programa deja que el hilo termine.
        pthread_join(hilo_dfs, NULL);

        FILE *output = fopen(tabla_indice, "w");
        fprintf(output, "%s", archivo_indice);
        fclose(output);
    }
	return 0;
}