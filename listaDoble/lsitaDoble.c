#include "listaDoble.h"


void crearListaDoble(tListaD* pl)
{
    *pl = NULL;
}
int vaciarListaDoble(tListaD* pl)
{
    int cant = 0;
    tNodoD *act = *pl;
    tNodoD *nae;

    if(act)
    {
        while(act->ant)
            act = act->ant;

        while(act)
        {
            nae = act->sig;

            free(act->info);
            free(act);
            act = nae;
            cant++;
        }

        *pl = NULL;
    }

    return cant;
}

int listaDobleVacia(const tListaD* pl)
{
    return(*pl == NULL);
}

int listaDobleLlena(const tListaD* pl, unsigned cantBytes)
{
    tNodoD *nue = (tNodoD*)malloc(sizeof(tNodoD));
    void *info = malloc(cantBytes);

    free(nue);
    free(info);

    return(info == NULL || nue == NULL);
}
int ponerAlInicioListaDoble(tListaD* pl, const void* dato, unsigned cantBytes)
{
    tNodoD *act = *pl,
            *nue;

    if(act)
    {
        while(act->ant)
            act = act->ant;

        nue = (tNodoD*)malloc(sizeof(tNodoD));

        if(nue == NULL)
            return SIN_MEM;

        nue->info = malloc(cantBytes);

        if(nue->info == NULL)
        {
            free(nue);
            return SIN_MEM;
        }
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = act;
    nue->ant = NULL;

    if(act)
        act->ant = nue;

    *pl = nue;

    return TODO_OK;
}
int ponerAlFinalListaDoble(tListaD* pl, const void* dato, unsigned cantBytes)
{
    tNodoD *act = *pl,
            *nue;

    if(act)
    {
        while(act->sig)
            act = act->sig;

        nue = (tNodoD*)malloc(sizeof(tNodoD));

        if(nue == NULL)
            return SIN_MEM;

        nue->info = malloc(cantBytes);

        if(nue->info == NULL)
        {
            free(nue);
            return SIN_MEM;
        }
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    nue->ant = act;

    if(act)
        act->sig = nue;

    *pl = nue;

    return TODO_OK;

}
int ponerEnOrdenListaDoble(tListaD* pl, const void* dato, unsigned cantBytes, int(*comparar)(const void*, const void*),
                           int(*acumular)(void**, unsigned*, const void*, unsigned))
{
    tNodoD  *act = *pl,
             *sig,
             *ant,
             *nue;
    int comp;

    if(act == NULL)
    {
        ant = NULL;
        sig = NULL;
    }
    else
    {
        while(act->sig && comparar(act->info, dato) < 0)
            act = act->sig;

        while(act->ant && comparar(act->info, dato) > 0)
            act = act->ant;

        comp = comparar(act->info, dato);

        if(comp == 0)
        {
            *pl = act;

            if(acumular)
            {
                if(acumular(&act->info, &act->tamInfo, dato, cantBytes) == 0)
                    return SIN_MEM;
            }

            return DUPLICADO;
        }

        if(comp < 0)
        {
            ant = act;
            sig = act->sig;
        }
        else
        {
            ant = act->ant;
            sig = act;
        }
    }

    nue = (tNodoD*)malloc(sizeof(tNodoD));

    if(nue == NULL)
        return SIN_MEM;

    nue->info = malloc(cantBytes);

    if(nue->info == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = sig;
    nue->ant = ant;

    if(sig)
        sig->ant = nue;

    if(ant)
        ant->sig = nue;

    *pl = nue;

    return TODO_OK;
}

void ordenarLista(tListaD *pl, int(*comparar)(const void*, const void*))
{
    tNodoD  *act = *pl,
             *sup = NULL,
              *inf = NULL;
    int      marca = 1;
    void    *info;
    unsigned tam;

    if(act == NULL)
        return;

    while(act->ant)
        act = act->ant;

    while(marca)
    {
        marca = 0;

        while(act->sig != sup)
        {
            if(comparar(act->info, act->sig->info) > 0)
            {
                info = act->info;
                tam = act->tamInfo;

                marca = 1;

                /**Intercambio**/
                act->info = act->sig->info;
                act->sig->info = info;

                /**Intercambio**/
                act->tamInfo = act->sig->tamInfo;
                act->sig->tamInfo = tam;
            }

            act = act->sig;
        }

        sup = act;

        while(act->ant != inf)
        {
            if(comparar(act->info, act->ant->info) < 0)
            {
                info = act->info;
                tam = act->tamInfo;

                marca = 1;

                /**Intercambio**/
                act->info = act->ant->info;
                act->ant->info = info;

                /**Intercambio**/
                act->tamInfo = act->ant->tamInfo;
                act->ant->tamInfo = tam;
            }

            act = act->sig;
        }

        inf = act;
    }
}

int mostrarDeIzqDerListaDoble(tListaD* pl, void(*mostrar)(const void*))
{
    tNodoD *act = *pl;
    int cant = 0;

    if(act)
    {
        mostrar(NULL);

        while(act->ant)
            act = act->ant;

        while(act)
        {
            mostrar(act->info);
            act = act->sig;
            cant++;
        }
    }

    return cant;
}

int mostrarDeDerIzqListaDoble(tListaD* pl, void(*mostrar)(const void*))
{
    tNodoD *act = *pl;
    int cant = 0;

    if(act)
    {
        mostrar(NULL);

        while(act->sig)
            act = act->sig;

        while(act)
        {
            mostrar(act->info);
            act = act->ant;
            cant++;
        }
    }

    return cant;
}
int eliminarPorClave(tListaD *pl, void* dato, unsigned cantBytes, int(*comparar)(const void*, const void*))
{
    tNodoD  *act = *pl,
            *ant,
            *sig;
    int     comp;

    if(act == NULL)
        return LISTA_VACIA;

    while(act->sig && comparar(act->info, dato) < 0)
        act = act->sig;

    while(act->ant && comparar(act->info, dato) > 0)
        act = act->ant;

    comp = comparar(act->info, dato);

    if(comp == 0) /**Encontro la clave**/
    {
        ant = act->ant;
        sig = act->sig;

        if(ant)
            ant->sig = sig;

        if(sig)
        {
            sig->ant = ant;
            *pl = sig;
        }else
        {
            *pl = ant;
        }

        memcpy(dato, act->info, cantBytes);
        free(act->info);
        free(act);

        return TODO_OK;
    }

    return NO_EXISTE;
}
