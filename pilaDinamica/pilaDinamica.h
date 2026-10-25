#ifndef PILADINAMICA_H_INCLUDED
#define PILADINAMICA_H_INCLUDED


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

void crearPila(tPila* pp);

int ponerEnPila(tPila* pp, const void* pd, unsigned tam);
int sacarDePila(tPila* pp, void* pd, unsigned tam);
int verTopePila(tPila* pp, void* pd, unsigned tam);

int pilaVacia(const tPila* pp);
int pilaLlena(const tPila* pp, unsigned tam);

void vaciarPila(tPila* pp);

#endif // PILADINAMICA_H_INCLUDED
