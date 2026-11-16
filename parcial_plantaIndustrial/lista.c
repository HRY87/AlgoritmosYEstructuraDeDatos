#include "lista.h"

void crearLista(tLista *pl)
{
    *pl = NULL;
}

/* Inserta ordenado ascendente. Si hay claves iguales, el nuevo queda
   despues de los existentes (no se pierde ningun registro). */
int ponerEnOrdenLista(tLista *pl, const void *dato, unsigned cantBytes,
                      int(*comparar)(const void*, const void*))
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

void recorrerLista(tLista *pl, void(*accion)(void*, void*), void *param)
{
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
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
