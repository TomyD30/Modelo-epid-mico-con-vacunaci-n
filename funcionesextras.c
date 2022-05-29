#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h> // API del Sistema Operativo de Windows (Permite trabajar sobre la Consola).
//#include <stdarg.h> //para argumentos tipo ...
#include "mtwister.h"

MTRand randGen ;

void inicializar_numeros_aleatorios(){
    randGen = seedRand(time(NULL));
    for(int i = 0 ; i < 100 ; i++) genRand(&randGen);
}

float Rrandom(double min , double max){
    float r = (max-min)*genRand(&randGen) + min ;
//    while(r == excepto) r = (max-min)*1.0*rand()/RAND_MAX + min ;

    return r ;
}

int Zrandom(int min , int max , int excepto){ //hay forma de hacer que "excepto" sea opcional ? ver argumentos tipo ...
//    int z = rand()%(max-min+1) + min ; // esta forma no sirve porque rand() tiene un numero maximo RAND_MAX y entonces rand()%n para n mayores a RAND_MAX solo generan numeros desde 0 hasta RAND_MAX y no hasta n
    int z = (max-min+1)*genRand(&randGen) + min ;

    while(z == excepto || z == max+1) z = (max-min+1)*genRand(&randGen) + min ;

    return z ;
}

//void inicializar_numeros_aleatorios(){ //para rand()
//    srand(time(NULL));
//    for(int i = 0 ; i < 100 ; i++) rand();
//}

int poissonRandom(float valorMedio , int kMin , int kMax){
    float L = exp(-valorMedio) , x = 1.0 ;
    int k = 0 ;

    while(x > L){
        k++ ;
        x *= Rrandom(0 , 1);
    }

    k-- ;

    if(k >= kMin && k <= kMax) return k ;
    else return poissonRandom(valorMedio , kMin , kMax) ;
}

//enum Colors { // Listado de colores (La letra "L" al inicio, indica que es un color más claro que su antecesor).
// BLACK = 0,
// BLUE = 1,
// GREEN = 2,
// CYAN = 3,
// RED = 4,
// MAGENTA = 5,
// BROWN = 6,
// LGREY = 7,
// DGREY = 8,
// LBLUE = 9,
// LGREEN = 10,
// LCYAN = 11,
// LRED = 12,
// LMAGENTA = 13,
// YELLOW = 14,
// WHITE = 15
//};

void Color(int Background, int Text){ // Función para cambiar el color del fondo y/o pantalla

 HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos la consola.

 // Para cambiar el color, se utilizan números desde el 0 hasta el 255.
 // Pero, para convertir los colores a un valor adecuado, se realiza el siguiente cálculo.
 int    New_Color= Text + (Background * 16);

 SetConsoleTextAttribute(Console, New_Color); // Guardamos los cambios en la Consola.

}

void barra_de_carga(int m , int n){
    for(int i = 0 ; i < 108 ; i++) printf("\b");
    float porcentaje ;
    porcentaje = 1.0*m*(100.0/n);
    if(m == n) Color(0 , 10);
    printf("[");
    for(float i = 0 ; i < porcentaje ; i += 1.0){
        printf("/");
    }
    for(float i = porcentaje ; i < 100 ; i += 1.0){
        printf("_");
    }
    printf("]%.1f%%",porcentaje);
    if(m == n) Color(0 , 15);
}

void barra_de_carga_fallida(){
    for(int i = 0 ; i < 108 ; i++) printf("\b");
    Color(0 , 4);
    printf("[");
    for(float i = 0 ; i < 16  ; i += 1.0){
        printf("2a3");
    }
    for(float i = 0 ; i < 17 ; i += 1.0){
        printf("bq7");
    }
    printf("]ERROR!!");
    Color(0 , 15);
}

//void chequear_memoria(char nombre_memoria[] , ...){
//    va_list memoria ;
//    va_start(memoria , nombre_memoria);
//
//    printf("%s",nombre_memoria);
//
//    va_end(memoria);
//}



