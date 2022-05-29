#ifndef REDESCOMPLEJAS_H
#define REDESCOMPLEJAS_H

#define susceptible 0
#define infectado 1
#define recuperado 2
#define vacunado 3

typedef struct conexion{
    int conectadocon ; //numero del nodo con el que esta conectado
    int tipo ; //0 para no dirigida | 1 para dirigida
    int fuerza ; //fuerza de la conexion
} conexion ;

typedef struct nodo{
    int numero ; //número que identifica al nodo
    int num_conexiones ; //cantidad de nodos con los que está conectado
    int conexiones_tomadas ; //cantidad de conexiones ya conectadas
    conexion *conexiones ; //nodos con los que esta conectado
    int estado ; //ver definiciones en la parte de arriba
    int t_infeccion ;
} nodo ;

typedef struct infeccion{
    float beta ; //infectividad
    int nInfectados ;
    int infectadosPrevios ;
} infeccion ;

typedef struct vacunacion{
    float prob ; //probabilidad de que un nodo acepte la vacuna
    int nVacunados ;
    int *rechazada ;
    int rechazadas ;
} vacunacion ;

typedef struct red{
    int n ; //numero total de nodos
    nodo *nodos ;
    int conex_totales ; //conexiones totales en la red
    float valorMedioConexiones ; //valor medio de las conexiones
    int minConexiones ;
    int maxConexiones ;
    int *distribucion_conexiones ;
    int *nodos_sin_conectar ;
    int *infectados ;
    int n_susceptibles ;
    int n_infectados ;
    int n_recuperados ;
    int n_vacunados ;
    infeccion inf ;
    vacunacion vac ;

    int t ; //tiempo (por ahora usado para infeccion)
} red ;

/// FUNCIONES

void iniciar_programa(int , char*[]);

/// Crea una red de n nodos
void crear_red(red** , int n);

/// Genera (aleatoriamente) la cantidad de conexiones para cada nodo con un valorMedio, y con un minimo y un maximo
void crear_conexiones(red*, float valorMedio , int min , int max);

int sortear_num_conexiones(red*);

/// Desconecta todos los nodos
void borrar_conexiones(red*);

void conectar_red(red*);

int conectar_nodos(red*);

void crear_nodos_sin_conectar(red*);

/// Se fija si los nodos a y b están conectados
int conectados(red* , int a, int b);

/// Imprime en pantalla las conexiones de cada nodo
void info_final(red* , char);

/// Copia en el archivo "datos nodos.txt" los nodos y sus conexiones (primera columna el nodo y las siguientes las conexiones)
void copiar_datos_conexiones(red* , char);

/// Copia en el archivo "distribucion de conexiones.txt" la cantidad de nodos que tienen cierto número de conexiones
void copiar_datos_distribucion(red* , char);

/// Pregunta al usuario si quiere saber si dos nodos están conectados
void info_conectados(red* , char); //por qué acá tengo que poner char ??

/// FUNCIONES DE INFECCION ///

void comenzar_infeccion(red* , char , int);

void infectar(red* , float);

void reiniciar_infeccion(red*);

void recuperar(red*);

void vacunar(red* , float);

void realizar_experimento(red*);

void info_infeccion_final(red*);

/// FUNCIONES DE MEMORIA ///

void asignar_memoria_red(red** , int);
void asignar_memoria_conexiones(red*);
void asignar_memoria_nodos_sin_conectar(red*);
void asignar_memoria_infeccion(red*);
void asignar_memoria_vacunacion(red*);

#endif
