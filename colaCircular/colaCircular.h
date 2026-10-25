#ifndef COLACIRCULAR_H_INCLUDED
#define COLACIRCULAR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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

/**No cambia su logica**/
void crearColaCircular(tCola* pc);
int colaCircularVacia(const tCola* pc);
int colaCircularLlena(const tCola* pc, unsigned tam);

/**Cambia su logica**/
int ponerColaCircular(tCola* pc, const void* pd, unsigned tam);
int sacarColaCircular(tCola* pc, void* pd, unsigned tam);
int verTopeColaCircular(tCola* pc, void* pd, unsigned tam);
void vaciarColaCircular(tCola* pc);

#endif // COLACIRCULAR_H_INCLUDED
