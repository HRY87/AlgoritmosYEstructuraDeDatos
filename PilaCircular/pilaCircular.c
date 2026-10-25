#include "pilaCircular.h"

void crearPilaCircular(tPila* pp)
{
    *pp = NULL;
}

int pilaCircularVacia(const tPila* pp)
{
    return (*pp == NULL);
}

int pilaCircularLlena(const tPila* pp, unsigned tam)
{
    void* nodo = malloc(sizeof(tNodo));
    void* info = malloc(tam);

    //LIBERA_MEMORIA_PERO_SIGUE_APUNTANDO_A_LA_DIRECCION
    free(nodo);
    free(info);

    return (!nodo || !info);
}

int ponerPilaCircular(tPila* pp, const void* pd, unsigned tam)
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
    mempcpy(nue->info, pd, tam);

    /**Si la pila tiene elementos**/
    if(*pp)
    {
        nue->sig = (*pp)->sig; //El nuevo nodo apunta al ultimo insertado
        (*pp)->sig = nue; //El primer insertado, apunta al nuevo elemento.
    }else /**Si la pila esta vacia**/
    {
        nue->sig = nue; //Primer nodo en la pila: se apunta a si mismo
        *pp = nue; //El puntero a pila apunta al unico nodo
    }

    return TODO_OK;
}

int sacarPilaCircular(tPila* pp, void* pd, unsigned tam)
{
    tNodo* nae = *pp;

    if(!*pp)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    nae = (*pp)->sig; //Nos ubicamos en el tope

    memcpy(pd, nae->info, MINIMO(nae->tamInfo, tam)); //Copiamos la info en pd

    /**Si la pila tiene un solo nodo**/
    if(nae == *pp)
    {
        *pp = NULL; //Queda vacia
    }else
    {
        (*pp)->sig = nae->sig; //El tope apuntara al siguiente de nodo a eliminar (nae)
    }

    free(nae->info);
    free(nae);

    return TODO_OK;
}

int verTopePilaCircular(tPila* pp, void* pd, unsigned tam)
{
    if(!*pp)
    {
        return PILA_VACIA; //PILA_VACIA
    }

    memcpy(pd, (*pp)->info, MINIMO((*pp)->sig->tamInfo, tam));

    return TODO_OK;
}

void vaciarPilaCircular(tPila* pp)
{
    tNodo* nae;

    while(*pp)
    {
        nae = (*pp)->sig; //Nos ubicamos en el ultimo nodo

        /**Si queda un solo nodo**/
        if(nae == *pp)
        {
            *pp = NULL; //La pila queda vacia
        }else
        {
            (*pp)->sig = nae->sig; //Pasamos al siguiente nodo
        }

        free(nae->info);
        free(nae);
    }
}
