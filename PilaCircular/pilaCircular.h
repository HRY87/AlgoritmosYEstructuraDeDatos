#ifndef PILACIRCULAR_H_INCLUDED
#define PILACIRCULAR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MINIMO(X,Y)     ((X)<(Y)?(X):(Y))

#define SIN_MEM         -1
#define TODO_OK         0
#define PILA_VACIA      1

typedef struct sNodo
{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tPila;

/**No cambia su logica**/
void crearPilaCircular(tPila* pp);
int pilaCircularVacia(const tPila* pp);
int pilaCircularLlena(const tPila* pp, unsigned tam);

/**Cambia su logica**/
int ponerPilaCircular(tPila* pp, const void* pd, unsigned tam);
int sacarPilaCircular(tPila* pp, void* pd, unsigned tam);
int verTopePilaCircular(tPila* pp, void* pd, unsigned tam);
void vaciarPilaCircular(tPila* pp);

#endif // PILACIRCULAR_H_INCLUDED
