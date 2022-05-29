
void inicializar_numeros_aleatorios();

/// Genera un numero real aleatorio entre min y max
float Rrandom(double min , double max);

/// Genera un número entero aleatorio entre min y max , quitando un número "excepto"
int Zrandom(int min , int max , int excepto);

/// Genera un número con una distribución de Poisson con un valor medio entre kMin y kMax
int poissonRandom(float valorMedio , int kMin , int kMax);

void Color(int , int);

/// Barra de carga completa hasta el paso m para un proceso de n pasos
void barra_de_carga(int m, int n);

void barra_de_carga_fallida();

//void chequear_memoria();
