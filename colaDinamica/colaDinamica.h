#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(X,Y) ((X)<(Y)? (X):(Y))
#define SIN_MEM         -1
#define TODO_OK         0
#define COLA_LLENA      1
#define COLA_VACIA      2

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig;
}tNodo;

typedef struct
{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crearCola(tCola *cola);
int ponerEnCola(tCola *cola, const void* dato, unsigned cantBytes);
int sacarDeCola(tCola *cola, void* dato, unsigned cantBytes);
int verPrimeroCola(const tCola *cola,void* dato, unsigned cantBytes);
void vaciarCola(tCola *cola);
int colaVacia(const tCola *cola);
int colaLlena(const tCola *cola, unsigned cantBytes);

#endif // COLADINAMICA_H_INCLUDED
