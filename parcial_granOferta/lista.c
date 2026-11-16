#include "lista.h"

void crearLista(tLista *pl)
{
    *pl = NULL;
}

int listaVacia(const tLista *pl)
{
    return (*pl == NULL);
}

void vaciarLista(tLista *pl)
{
    while(*pl)
    {
        tNodo *aux = *pl;

        *pl = aux->sig;
        free(aux->info);
        free(aux);
    }
}

/** Inserta ordenado; si hay claves iguales, queda DESPUES de ellas (estable) **/
int ponerListaOrdenadaDup(tLista *pl, const void *dato, unsigned cantBytes,
                          int (*comparar)(const void*, const void*))
{
    tNodo *nue;

    while(*pl && comparar(dato, (*pl)->info) >= 0)
        pl = &(*pl)->sig;

    nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
        return SIN_MEM;

    nue->info = malloc(cantBytes);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *pl;
    *pl = nue;

    return TODO_OK;
}

int verUltimoLista(const tLista *pl, void *dato, unsigned cantBytes)
{
    if(*pl == NULL)
        return LISTA_VACIA;

    while((*pl)->sig)
        pl = &(*pl)->sig;

    memcpy(dato, (*pl)->info, MINIMO(cantBytes, (*pl)->tamInfo));

    return TODO_OK;
}

/** Con la lista ordenada y n >= 1: busca el n-esimo nodo y elimina todos los
    que le siguen y son DISTINTOS a el (los iguales son empates y se conservan).
    Devuelve la cantidad de nodos eliminados. **/
unsigned recortarListaMayoresAlNEsimo(tLista *pl, unsigned n,
                                      int (*comparar)(const void*, const void*))
{
    tNodo *nEsimo,
          *nae;
    unsigned cant = 0;

    while(*pl && n > 1)
    {
        pl = &(*pl)->sig;
        n--;
    }

    if(!*pl)
        return 0;

    nEsimo = *pl;
    pl = &nEsimo->sig;

    while(*pl && comparar(nEsimo->info, (*pl)->info) == 0)
        pl = &(*pl)->sig;

    while(*pl)
    {
        nae = *pl;
        *pl = nae->sig;

        free(nae->info);
        free(nae);
        cant++;
    }

    return cant;
}

void recorrerLista(tLista *pl, void (*accion)(void*, void*), void *param)
{
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}

void reduce(tLista *pl, void *result, void (*accion)(void*, void*, void*), void *param)
{
    while(*pl)
    {
        accion((*pl)->info, result, param);
        pl = &(*pl)->sig;
    }
}
