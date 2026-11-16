#include "colaCircular.h"

void crearColaCircular(tCola* pc)
{
    *pc = NULL;
}

int ponerColaCircular(tCola* pc, const void* pd, unsigned tam)
{
    tNodo* nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
        return SIN_MEM;

    nue->info = malloc(tam);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    nue->tamInfo = tam;
    memcpy(nue->info, pd, tam);

    /**Si la cola tiene elementos**/
    if(*pc)
    {
        nue->sig = (*pc)->sig; //El nuevo nodo apunta al primer insertado
        (*pc)->sig = nue;      //El ultimo insertado apunta al nuevo
    }
    else /**Si la cola esta vacia**/
    {
        nue->sig = nue;        //El nuevo nodo apunta hacia si mismo
    }

    *pc = nue; //El nuevo nodo se convierte en el ultimo

    return TODO_OK;
}

int verTopeColaCircular(tCola* pc, void* pd, unsigned tam)
{
    if(!*pc)
        return COLA_VACIA;

    /**El primero de la cola es el siguiente al ultimo**/
    memcpy(pd, (*pc)->sig->info, MINIMO((*pc)->sig->tamInfo, tam));

    return TODO_OK;
}

void vaciarColaCircular(tCola* pc)
{
    tNodo* nae;

    while(*pc)
    {
        nae = (*pc)->sig; //Nos ubicamos en el primero

        /**Si queda un solo nodo**/
        if(nae == *pc)
            *pc = NULL;
        else
            (*pc)->sig = nae->sig;

        free(nae->info);
        free(nae);
    }
}
