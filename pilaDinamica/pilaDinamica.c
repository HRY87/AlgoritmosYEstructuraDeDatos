#include "pilaDinamica.h"

void crearPila(tPila* pp)
{
    *pp = NULL;
}

int ponerEnPila(tPila* pp, const void* pd, unsigned tam)
{
    tNodo* nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
    {
        return SIN_MEM; //PILA_LLENA
    }

    nue->info = malloc(tam);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM; //PILA_LLENA
    }

    nue->tamInfo = tam;
    memcpy(nue->info, pd, tam);

    nue->sig = *pp;

    *pp = nue;

    return TODO_OK;
}

int sacarDePila(tPila* pp, void* pd, unsigned tam)
{
    tNodo* elim = *pp;

    if(!*pp)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    memcpy(pd, elim->info, MINIMO(elim->tamInfo, tam));
    *pp = elim->sig;

    free(elim->info);
    free(elim);

    return TODO_OK;
}

int verTopePila(tPila* pp, void* pd, unsigned tam)
{
    if(!*pp)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    memcpy(pd, (*pp)->info, MINIMO((*pp)->tamInfo, tam));

    return TODO_OK;
}

int pilaVacia(const tPila* pp)
{
    return (*pp == NULL);
}

int pilaLlena(const tPila* pp, unsigned tam)
{
    void* nodo = malloc(sizeof(tNodo));
    void* info = malloc(tam);

    //LIBERA_MEMORIA_PERO_SIGUE_APUNTANDO_A_LA_DIRECCION
    free(nodo);
    free(info);

    return (!nodo || !info);
}

void vaciarPila(tPila* pp)
{
    tNodo* elim;

    while(*pp)
    {
        elim = *pp;
        *pp = elim->sig;
        free(elim->info);
        free(elim);
    }
}
