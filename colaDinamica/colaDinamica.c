#include "colaDinamica.h"

void crearCola(tCola *cola)
{
    cola->pri = NULL;
    cola->ult = NULL;
}

int ponerEnCola(tCola *cola, const void* dato, unsigned cantBytes)
{
    tNodo* nue = (tNodo*)malloc(sizeof(tNodo));

    nue->info = malloc(cantBytes);

    if(nue == NULL || nue->info == NULL)
    {
        free(nue);
        return COLA_LLENA;
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;

    /**Si ult es igual a NULL, es el primer elemento*/
    if(cola->ult)
        cola->ult->sig = nue;
    else
        cola->pri = nue;

    cola->ult = nue;

    return TODO_OK;

}

int sacarDeCola(tCola *cola, void* dato, unsigned cantBytes)
{
    tNodo* nae = cola->pri;

    if(nae == NULL)
        return COLA_VACIA;

    /**Asigno el siguiente a la primera posicion**/
    cola->pri = nae->sig;

    memcpy(dato, nae->info, MINIMO(nae->tamInfo, cantBytes));

    free(nae->info);
    free(nae);

    if(cola->pri == NULL)
        cola->ult = NULL;

    return TODO_OK;
}

int verPrimeroCola(const tCola *cola,void* dato, unsigned cantBytes)
{
    if(cola->pri == NULL)
        return COLA_VACIA;

    memcpy(dato, cola->pri->info, MINIMO(cantBytes, cola->pri->tamInfo));

    return TODO_OK;
}

void vaciarCola(tCola *cola)
{
    while(cola->pri)
    {
        tNodo *aux = cola->pri;
        cola->pri = aux->sig;

        /**Primero la info, despues el nodo***/
        free(aux->info);
        free(aux);
    }

    cola->ult = NULL;
}

int colaVacia(const tCola *cola)
{
    return cola->pri == NULL;
}

int colaLlena(const tCola *cola, unsigned cantBytes)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(cantBytes);

    free(aux);
    free(info);

    return (aux == NULL || info == NULL);
}
