#include "colaCircular.h"

void crearColaCircular(tCola* pc)
{
    *pc = NULL;
}

int colaCircularVacia(const tCola* pc)
{
    return (*pc == NULL);
}

int colaCircularLlena(const tCola* pc, unsigned tam)
{
    void* nodo = malloc(sizeof(tNodo));
    void* info = malloc(tam);

    //LIBERA_MEMORIA_PERO_SIGUE_APUNTANDO_A_LA_DIRECCION
    free(nodo);
    free(info);

    return (!nodo || !info);
}

int ponerColaCircular(tCola* pc, const void* pd, unsigned tam)
{
    tNodo* nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
    {
        return SIN_MEM; //COLA_LLENA
    }

    nue->info = malloc(tam);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM; //COLA_LLENA
    }

    nue->tamInfo = tam;
    mempcpy(nue->info, pd, tam);

    /**Si la cola tiene elementos**/
    if(*pc)
    {
        nue->sig = (*pc)->sig; //El nuevo nodo apunta al primer insertado
        (*pc)->sig = nue; //El ultimo insertado apunta al nuevo
    }else /**Si la cola esta vacia**/
    {
        nue->sig = nue; //El nuevo nodo apunta hacia si mismo
    }

    *pc = nue; //El nuevo nodo se convierte en el ultimo
    return TODO_OK;
}

int sacarColaCircular(tCola* pc, void* pd, unsigned tam)
{
    tNodo* nae = *pc;

    if(!*pc)
    {
        return COLA_VACIA; //COLA_VACIA
    }

    nae = (*pc)->sig; //Nos ubicamos en el tope

    memcpy(pd, nae->info, MINIMO(nae->tamInfo, tam)); //Copiamos la info en pd

    /**Si la cola tiene un solo nodo**/
    if(nae == *pc)
    {
        *pc = NULL; //Queda vacia
    }else
    {
        (*pc)->sig = nae->sig; //El tope apuntara al siguiente de nodo a eliminar (nae)
    }

    free(nae->info);
    free(nae);

    return TODO_OK;
}

int verTopeColaCircular(tCola* pc, void* pd, unsigned tam)
{
    if(!*pc)
    {
        return COLA_VACIA; //COLA_VACIA
    }

    memcpy(pd, (*pc)->info, MINIMO((*pc)->sig->tamInfo, tam));

    return TODO_OK;
}

void vaciarColaCircular(tCola* pc)
{
    tNodo* nae;

    while(*pc)
    {
        nae = (*pc)->sig; //Nos ubicamos en el ultimo nodo

        /**Si queda un solo nodo**/
        if(nae == *pc)
        {
            *pc = NULL; //La cola queda vacia
        }else
        {
            (*pc)->sig = nae->sig; //Pasamos al siguiente nodo
        }

        free(nae->info);
        free(nae);
    }
}
