#ifndef COLAESTATICA_H_INCLUDED
#define COLAESTATICA_H_INCLUDED

#define MINIMO(X,Y)         ((X)<(Y)?(X):(Y))

#define TODO_OK             0
#define COLA_LLENA          1
#define COLA_VACIA          2
#define COLA_NO_VACIA       3
#define COLA_DISPONIBLE     4

#define TAM_COLA            400 //CANTIDAD_DE_BYTES_DISPONIBLES
typedef struct
{
    char        cola[TAM_COLA];
    unsigned    pri,
                ult,
                tamDisp;
}tCola;

void crearCola(tCola *cola);
int ponerEnCola(tCola *cola,const void* dato,unsigned tam);
int sacarDeCola(tCola *cola,void* dato,unsigned tam);
int verPrimeroCola(const tCola *cola,void* dato,unsigned tam);
void vaciarCola(tCola *cola);
int colaVacia(const tCola *cola);
int colaLlena(const tCola *cola, unsigned tam);

#endif // COLAESTATICA_H_INCLUDED
