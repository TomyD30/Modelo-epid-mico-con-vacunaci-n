#include "redescomplejas.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "funcionesextras.h"

FILE *datos , *infoinfectados , *configuracion_anterior , *tiempo , *configInfeccion , *infeccion_t ;
char *archivoTiempo ;

int n , min , max ;
float vm ;

int t_r ;
float beta , betaMin , betaMax , betaSaltos ;
char *nombreArchivo ;

float vacProb ;

extern char infoextra ;

int erroresMemoria ;

void iniciar_programa(int argc , char *argv[]){
    inicializar_numeros_aleatorios();

    n = atoi(argv[1]) , vm = atof(argv[2]) , min = atoi(argv[3]) , max = atoi(argv[4]);
    beta = atof(argv[5]) , t_r = atoi(argv[6]) , vacProb = atof(argv[7]) ;
    nombreArchivo = strdup(argv[8]) , archivoTiempo = strdup(argv[9]) ;
    tiempo = fopen(archivoTiempo , "w");
    infeccion_t = fopen(nombreArchivo , "w");
}

void crear_red(red **lared , int n){

    asignar_memoria_red(lared , n);

    (*lared)->n = n ;
    (*lared)->conex_totales = 0 ;

    //
    fprintf(configuracion_anterior , "%d\n",n);
    //

}

void crear_conexiones(red *lared , float valorMedio , int min , int max){

    printf("Conexion al: \n");
    barra_de_carga(0 , 3);

    lared->valorMedioConexiones = valorMedio ;
    lared->minConexiones = min ;
    lared->maxConexiones = max ;

    for(int i = 0 ; i < lared->n ; i++) lared->nodos[i].conexiones_tomadas = 0 ;

    lared->distribucion_conexiones = calloc(lared->maxConexiones , sizeof(int));

    int j ;

    j = sortear_num_conexiones(lared);

    int contador = 0 , limite = 100 ;

    while(j == 0){
        borrar_conexiones(lared);
        j = sortear_num_conexiones(lared);
        contador++ ;
        if(contador == limite){
            printf("No se pudieron crear las conexiones...\n") , exit(3004);
            break ;
        }
    }

    barra_de_carga(1 , 3);

    //
    fprintf(configuracion_anterior , "%f\n%d\n%d\n", valorMedio , min , max);
    //
}

int sortear_num_conexiones(red *lared){

    int control = 1 ;

    for(int i = 0 ; i < lared->n ; i++){
        lared->nodos[i].num_conexiones = poissonRandom(lared->valorMedioConexiones , lared->minConexiones , lared->maxConexiones) ;
        lared->distribucion_conexiones[lared->nodos[i].num_conexiones]++ ;
        lared->conex_totales += lared->nodos[i].num_conexiones ;
    }
    asignar_memoria_conexiones(lared);

    //control (la cantidad de conexiones totales no puede ser impar)
    if(lared->conex_totales%2 != 0) control = 0;

    return control ;
}

void borrar_conexiones(red *lared){
    for(int i = 0 ; i < lared->n ; i++){
        lared->nodos[i].num_conexiones = 0 ; //esto hace falta? el free no los pone a 0 ya?
        free(lared->nodos[i].conexiones);
    }
    lared->conex_totales = 0 ;

    for(int i = 0 ; i < lared->maxConexiones ; i++) lared->distribucion_conexiones[i] = 0 ;

}

void conectar_red(red *lared){

    asignar_memoria_nodos_sin_conectar(lared);

    crear_nodos_sin_conectar(lared); //no puedo crearlo solo una vez?

    barra_de_carga(2 , 3);

    int j ; //1 si la conexion es exitosa , 0 si no

    j = conectar_nodos(lared);

    int contador = 0 , limite = 100*lared->n ;

    while (j == 0){
        crear_nodos_sin_conectar(lared);
        j = conectar_nodos(lared);
        contador ++ ;
        if(contador == limite){
            printf("La conexion no es posible :(\n Intentelo otra vez\n");
            exit(3004);
        }
    }

    free(lared->nodos_sin_conectar);

    barra_de_carga(3 , 3);

    if(j == 1) Color(0 , 10) , printf("\n->Los nodos se conectaron satisfactoriamente<-\n\n ") , Color(0 , 15);

}

int conectar_nodos(red *lared){
    int control = 1 ;

    int z1 , z2 , a , b , contador = 0 , limite = 100*lared->n ;

    for(int sinconectar = lared->conex_totales ; sinconectar != 0 ; sinconectar -= 2){

        if(infoextra == 's'){
            printf("nodos sin conectar:");
            for(int i = 0 ; i < sinconectar ; i++){
                printf("%d  ",lared->nodos_sin_conectar[i]);
            }
            printf("\n");
        }

        a = Zrandom(0 , sinconectar-1 , -1) , b = Zrandom(0 , sinconectar-1 , -1);
        z1 = lared->nodos_sin_conectar[a] , z2 = lared->nodos_sin_conectar[b] ;
        while(z1 == z2 || conectados(lared , z1 , z2) == 1){
            a = Zrandom(0 , sinconectar-1 , -1) , b = Zrandom(0 , sinconectar-1 , -1);
            z1 = lared->nodos_sin_conectar[a] , z2 = lared->nodos_sin_conectar[b] ;
            //control de posibles fallos
            if (z1 == z2) contador++ ;
            if(contador == limite) {
                if(infoextra == 's') printf("no quedan conexiones disponibles\n Reintentando conexion...\n");
                break ;
            }
        }

        if(contador == limite){
            contador = 0 , control = 0 ;
            break ;
        }

        if(infoextra == 's') printf("%d se conecto con %d\n",z1,z2);

        lared->nodos[z1].conexiones[lared->nodos[z1].conexiones_tomadas].conectadocon = z2 , lared->nodos[z1].conexiones_tomadas++ ;
        lared->nodos[z2].conexiones[lared->nodos[z2].conexiones_tomadas].conectadocon = z1 , lared->nodos[z2].conexiones_tomadas++ ;

        //se edita el vector nodos_sin_conectar
        if(b == sinconectar-1){
            lared->nodos_sin_conectar[b] = lared->nodos_sin_conectar[sinconectar-1];
            lared->nodos_sin_conectar[a] = lared->nodos_sin_conectar[sinconectar-2];
        }
        else{
            lared->nodos_sin_conectar[a] = lared->nodos_sin_conectar[sinconectar-1];
            lared->nodos_sin_conectar[b] = lared->nodos_sin_conectar[sinconectar-2];
        }
    }

    return control ;
}

void crear_nodos_sin_conectar(red *lared){
    int k = 0 ;
    for(int i = 0 ; i < lared->n ; i++){
        for(int j = 0 ; j < lared->nodos[i].num_conexiones ; j++){
            lared->nodos_sin_conectar[k] = i ;
            k++ ;
        }
    }
}

//void editar_nodos_sin_conectar(int *nodos_sin_conectar , int a , int b , int sinconectar){ //aca quiero trabajar con la copia de nodos sin conectar para que no modifique el verdadero
//
//}

int conectados(red *lared , int a , int b){
    int c = 0 ;
    for(int i = 0 ; i < lared->nodos[a].num_conexiones ; i++){
       if(lared->nodos[a].conexiones[i].conectadocon == b) c = 1 ;
       if(c == 1) break ;
    }
    return c ;
}

void info_final(red *lared , char si_o_no){
    if(si_o_no == 's'){
        for(int i = 0 ; i < lared->n ; i++){
            printf("|El nodo %d esta conectado a %d nodos|\n" , i , lared->nodos[i].num_conexiones);
            printf("Conexiones: ");
            for(int j = 0 ; j < lared->nodos[i].num_conexiones ; j++){
                printf("%d   " , lared->nodos[i].conexiones[j].conectadocon);
            }
            printf("\n--------------------------------------------\n");
        }
    }
}

void copiar_datos_conexiones(red *lared , char si_o_no){
    if(si_o_no == 's'){
        datos = fopen("datos nodos.txt","w");
        for(int i = 0 ; i < lared->n ; i++){
            fprintf(datos , "%d ", i);
            for(int j = 0 ; j < lared->nodos[i].num_conexiones ;j++){
                fprintf(datos, "%d " , lared->nodos[i].conexiones[j].conectadocon);
            }
            fprintf(datos, "\n");
        }
    }
}

void copiar_datos_distribucion(red *lared , char si_o_no){
    if(si_o_no == 's'){
        FILE *distribucion ;
        distribucion = fopen("distribucion de conexiones.txt","w");
        for(int i = 0 ; i <= lared->maxConexiones ; i++){
            fprintf(distribucion , "%d %d\n" , i , lared->distribucion_conexiones[i]);
        }
    }
}

void info_conectados(red *lared , char si_o_no){

        if(si_o_no == 's'){
        char seguir ;
        int a , b ;

        while(1){
            printf("nodo 1: ") , scanf("%d" , &a);
            printf("nodo 2: ") , scanf("%d" , &b);
            if(conectados(lared , a , b) == 1) printf("Estan conectados.\n");
            else printf("No estan conectados.\n");

            printf("Seguir? (s para si , n para no): ") , scanf(" %c" , &seguir);

            if(seguir == 's') printf("Eliga dos nuevos nodos.\n");
            else if(seguir == 'n'){
                printf("Finalizo el programa.\n");
                break ;
            }
            else printf("error");
        }
    }
    else if(si_o_no == 'n') printf("Finalizo el programa.\n");
    else printf("error");
}

void comenzar_infeccion(red *lared , char si_o_no , int argc){
    if(si_o_no == 's'){

        lared->n_susceptibles = lared->n ;
        lared->n_infectados = 0 ;
        lared->n_recuperados = 0 ;
        lared->vac.nVacunados = 0 ;
        lared->vac.rechazadas = 0 ;
        lared->t = 0 ;
        for(int i = 0 ; i < lared->n ; i++){
            lared->nodos[i].estado = susceptible ;
            lared->nodos[i].t_infeccion = 0 ;
        }
        asignar_memoria_infeccion(lared);

        asignar_memoria_vacunacion(lared);

        double tinicial = clock() ; //chequeo de tiempo que tarda el programa

        realizar_experimento(lared);

        double tfinal = clock() ;
        fprintf(tiempo,"tiempo gastado para t_r %d: %lf\n",t_r,(tfinal-tinicial)/CLOCKS_PER_SEC);

        fclose(infeccion_t) , fclose(tiempo);
    }
}

void infectar(red *lared , float beta){
    int nodoinfectado , vecino ;
    float p ;

    lared->inf.infectadosPrevios = lared->n_infectados ;

    for(int i = lared->inf.infectadosPrevios-1 ; i >= 0 ; i--){
        nodoinfectado = lared->infectados[i];
        for(int j = 0 ; j < lared->nodos[nodoinfectado].num_conexiones ; j++){
            vecino = lared->nodos[nodoinfectado].conexiones[j].conectadocon ;

            if(lared->nodos[vecino].estado == susceptible){
                p = Rrandom(0 , 1) ;
                if(p <= beta){
                    lared->nodos[vecino].estado = infectado ;
//                  printf("el nodo %d infecto a %d\n", nodoinfectado , vecino);
                    lared->infectados[lared->n_infectados] = vecino ;
                    lared->n_infectados++ ;
                    lared->n_susceptibles-- ;
                }
            }
        }
        lared->nodos[nodoinfectado].t_infeccion++ ;
    }
    lared->t++ ;
}

void reiniciar_infeccion(red *lared){
    lared->n_infectados = 0 ;
    lared->n_susceptibles = lared->n ;
    lared->n_recuperados = 0 ;
    lared->vac.nVacunados = 0 ;
    lared->vac.rechazadas = 0 ;
    for(int i = 0 ; i < lared->n ; i++){
        lared->nodos[i].estado = susceptible ;
        lared->nodos[i].t_infeccion = 0 ;
        lared->vac.rechazada[i] = 0 ;
//        lared->infectados[i] = 0 ;
    }
    lared->t = 0 ;
}

void recuperar(red *lared){
    int nodo ;
    for(int i = lared->inf.infectadosPrevios-1 ; i >= 0 ; i--){
        nodo = lared->infectados[i];
        if(lared->nodos[nodo].t_infeccion == t_r){
            lared->nodos[nodo].estado = recuperado ;
            lared->n_recuperados++ ;

            if(i != lared->n_infectados-1) lared->infectados[i] = lared->infectados[lared->n_infectados-1] ;
            lared->n_infectados-- ;
        }
    }
}

void vacunar(red *lared , float omega){
    float r ;
    int nodoinfectado , vecino ;
    lared->vac.prob = omega ;

    for(int i = 0 ; i < lared->inf.infectadosPrevios ; i++){
        nodoinfectado = lared->infectados[i];
        for(int j = 0 ; j < lared->nodos[nodoinfectado].num_conexiones ; j++){
            vecino = lared->nodos[nodoinfectado].conexiones[j].conectadocon ;
            if(lared->nodos[vecino].estado == susceptible){
                if(lared->vac.rechazada[vecino] == 0){
                    r = Rrandom(0 , 1);
                    if(r < lared->vac.prob){
                        lared->nodos[vecino].estado = vacunado ;
                        lared->vac.nVacunados++ ;
                        lared->n_susceptibles-- ;
                    }
                    else{
                        lared->vac.rechazada[vecino] = 1 ;
                        lared->vac.rechazadas++ ;
                    }
                }
            }
        }
    }
}

void realizar_experimento(red *lared){

    int nodoinfectado ;

    nodoinfectado = Zrandom(0 , lared->n-1 , -1) ;
    lared->nodos[nodoinfectado].estado = infectado ;
    lared->infectados[lared->n_infectados] = nodoinfectado  , lared->n_infectados++ , lared->n_susceptibles--;
//  printf("Paciente cero: %d (%d)\n" , nodoinfectado,lared->nodos[nodoinfectado].num_conexiones);

    while(lared->n_infectados != 0) {
        infectar(lared , beta);
        vacunar(lared , vacProb);
        recuperar(lared);

        printf("Dia %d:\n",lared->t);
        Color(0 , 4) , printf("Infectados: %d |" , lared->n_infectados);
        Color(0 , 1) , printf("Susceptibles: %d |" , lared->n_susceptibles);
        Color(0 , 2) , printf("Recuperados: %d |" , lared->n_recuperados);
        Color(0 , 3) , printf("Vacunados: %d\n" , lared->vac.nVacunados);
        Color(0 , 15);
        fprintf(infeccion_t,"%d %d %d %d %d\n",lared->t,lared->n_infectados,lared->n_susceptibles,lared->n_recuperados,lared->vac.nVacunados);
    }
}

void info_infeccion_final(red *lared){
    printf("\nDuracion de la enfermedad: %d dias\n",lared->t);
    printf("Porcentaje de la poblacion infectada: %.1f%%\n",100.0*lared->n_recuperados/lared->n);
    printf("Vacunas rechazadas: %d\n",lared->vac.rechazadas);
}

/// Funciones de Memoria

void asignar_memoria_red(red **lared , int n){
    *lared = malloc(sizeof(red));
    if(*lared == NULL) printf("error al asignar memoria a *lared\n") , erroresMemoria++ ;
    (*lared)->nodos = malloc(n*sizeof(nodo));
    if((*lared)->nodos == NULL) printf("error al asignar memoria a (*lared)->nodos\n") , erroresMemoria++ ;
}

void asignar_memoria_conexiones(red *lared){
    for(int i = 0 ; i < lared->n ; i++) {
        lared->nodos[i].conexiones = malloc(lared->nodos[i].num_conexiones*sizeof(conexion));
        if(lared->nodos[i].conexiones == NULL) printf("error al asignar memoria a lared->nodos[i].conexiones\n") , erroresMemoria++ ;
    }
}

void asignar_memoria_nodos_sin_conectar(red *lared){
    lared->nodos_sin_conectar = malloc(lared->conex_totales*sizeof(int));
    if(lared->nodos_sin_conectar == NULL) printf("error al asignar memoria a lared->nodos_sin_conectar\n") , erroresMemoria++ ;
}

void asignar_memoria_infeccion(red *lared){
    lared->infectados = calloc(lared->n , sizeof(int)) ;
    if(lared->infectados == NULL) printf("error al asignar memoria a lared->infectados\n") , erroresMemoria++ ;
}

void asignar_memoria_vacunacion(red *lared){
    lared->vac.rechazada = calloc(lared->n , sizeof(int));
    if(lared->vac.rechazada == NULL) printf("error al asignar memoria a lared->vac.rechazada\n") , erroresMemoria++ ;
}



