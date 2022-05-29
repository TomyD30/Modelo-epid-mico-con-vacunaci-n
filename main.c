#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "redescomplejas.h"
#include "funcionesextras.h"

red *mired ;

extern int n , min , max ;
extern float vm ;
char si_o_no , infoextra ;

int main(int argc , char *argv[]){

    iniciar_programa(argc , argv);

    crear_red(&mired , n);

    crear_conexiones(mired , vm , min , max);

//    printf("Imprimir informacion extra durante el proceso?(s para si , n para no): ") , scanf(" %c" , &infoextra);

    conectar_red(mired);

//    printf("Imprimir conexiones finales?(s para si , n para no): ") , scanf(" %c" , &si_o_no);
//
//    info_final(mired , si_o_no);
//
//    printf("Copiar datos finales?(s para si , n para no): ") , scanf(" %c", &si_o_no);
//
//    copiar_datos_conexiones(mired , si_o_no);
//
//    printf("Copiar datos de la distribucion de conexiones?(s para si , n para no): ") , scanf(" %c",&si_o_no);
//
//    copiar_datos_distribucion(mired , si_o_no);
//
//    printf("Queres saber si dos nodos estan conectados? (s para si , n para no): ") , scanf(" %c", &si_o_no);
//
//    info_conectados(mired , si_o_no);

//    printf("Comenzar infeccion? (s para si , n para no) :") , scanf(" %c" , &si_o_no);

    comenzar_infeccion(mired , 's' , argc);

}
