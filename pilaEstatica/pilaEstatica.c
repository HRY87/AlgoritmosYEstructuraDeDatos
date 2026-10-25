#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include "pilaEstatica.h"

void crearPila(tPila* pp)
{
    pp->tope = TAM_PILA;
}

int ponerEnPila(tPila* pp, const void* pd, unsigned tamDato)
{
    if(pp->tope < (tamDato * sizeof(unsigned)))
    {
        return SIN_MEM; //PILA_LLENA
    }

    pp->tope -= tamDato;
    memcpy(pp->pila + pp->tope, pd, tamDato);
    pp->tope -= sizeof(unsigned);
    memcpy(pp->pila + pp->tope, &tamDato, sizeof(unsigned));

    return TODO_OK;
}

int sacarDePila(tPila* pp, void* pd, unsigned tamDato)
{
    unsigned tamDatoPila;

    if(pp->tope >= TAM_PILA)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    memcpy(&tamDatoPila, pp->pila + pp->tope, sizeof(unsigned));
    pp->tope += sizeof(unsigned); // pp->tope = sizeof tamDatoPila
    memcpy(pd, pp->pila + pp->tope, MINIMO(tamDato, tamDatoPila));
    pp->tope += tamDatoPila;

    return TODO_OK;
}

int verTopePila(tPila* pp, void* pd, unsigned tamDato)
{
    unsigned tamDatoPila;
    if(pp->tope >= TAM_PILA)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    //NO_SE_MODIFICA_EL_DATO
    memcpy(&tamDatoPila, pp->pila+pp->tope, sizeof(unsigned));
    memcpy(pd, pp->pila+pp->tope, sizeof(unsigned));

    return SIN_MEM;
}

int pilaVacia(const tPila* pp)
{
    return (pp->tope >= TAM_PILA);
}
int pilaLlena(const tPila* pp, unsigned tamDato)
{
    return (pp->tope < (tamDato * sizeof(unsigned)));
}

void vaciarPila(tPila* pp)
{
    pp->tope = TAM_PILA;
}
