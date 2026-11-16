#ifndef COLACIRCULAR_H_INCLUDED
#define COLACIRCULAR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(X,Y)     ((X)<(Y)?(X):(Y))

#define SIN_MEM         -1
#define TODO_OK         0
#define COLA_VACIA      1

typedef struct sNodo
{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tCola;

/** Solo las primitivas que usa este parcial **/
void crearColaCircular(tCola* pc);
int ponerColaCircular(tCola* pc, const void* pd, unsigned tam);
int verTopeColaCircular(tCola* pc, void* pd, unsigned tam);
void vaciarColaCircular(tCola* pc);

#endif // COLACIRCULAR_H_INCLUDED
