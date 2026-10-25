#include "lista.h"

void crearLista(tLista *pl)
{
    *pl = NULL;
}

int listaVacia(const tLista *pl)
{
    return (*pl == NULL);
}

int listaLlena(const tLista *pl, unsigned cantBytes)
{
    tNodo *aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(cantBytes);

    free(aux);
    free(info);

    return (aux == NULL || info == NULL);
}

void vaciarLista(tLista *pl)
{
    while(*pl)
    {
        tNodo* aux = *pl;

        *pl = aux->sig;
        free(aux->info);
        free(aux);
    }
}

int ponerAlComienzoLista(tLista *pl, const void* dato, unsigned cantBytes)
{
    tNodo* nue;

    nue = (tNodo*)malloc(sizeof(tNodo));
    nue->info = malloc(cantBytes);

    if(nue == NULL || nue->info == NULL)
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

int sacarPrimeroLista(tLista *pl, void* dato, unsigned cantBytes)
{
    tNodo *aux = *pl;

    if(aux == NULL)
        return LISTA_VACIA;

    *pl = aux->sig;
    memcpy(dato, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);

    return TODO_OK;
}

int verPrimeroLista(const tLista *pl, void *dato, unsigned cantBytes)
{
    if(*pl == NULL)
        return LISTA_VACIA;

    memcpy(dato, (*pl)->info, MINIMO(cantBytes, (*pl)->tamInfo));
    return TODO_OK;
}

int ponerAlFinalLista(tLista* pl, const void* dato, unsigned cantBytes)
{
    tNodo* nue;

    while(*pl)
    {
        pl = &(*pl)->sig;
    }

    nue = (tNodo*)malloc(sizeof(tNodo));
    nue->info = malloc(cantBytes);

    if(nue == NULL || nue->info == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    *pl = nue;

    return TODO_OK;
}
int sacarUltimoLista(tLista *pl, void* dato, unsigned cantBytes)
{
    if(*pl == NULL)
        return LISTA_VACIA;

    while((*pl)->sig)
        pl = &(*pl)->sig;

    memcpy(dato, (*pl)->info, MINIMO(cantBytes, (*pl)->tamInfo));

    free((*pl)->info);
    free(*pl);
    *pl = NULL;

    return TODO_OK;
}

int verUltimoLista(const tLista *pl, void* dato, unsigned cantBytes)
{
    if(*pl == NULL)
        return LISTA_VACIA;

    while((*pl)->sig)
        pl = &(*pl)->sig;

    memcpy(dato, (*pl)->info, MINIMO(cantBytes, (*pl)->tamInfo));
    return TODO_OK;
}

int ponerPosicionLista(tLista* pl, void* dato, size_t tam, unsigned pos)
{
    tNodo* nue;

    while(*pl && pos)
    {
        pl = &(*pl)->sig;
        pos--;
    }

    /**Si es posicion invalida**/
    if(pos)
    {
        return NO_EXISTE;
    }

    nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
        return SIN_MEM;

    nue->info = malloc(tam);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    nue->tamInfo = tam;
    memcpy(nue->info, dato, tam);
    nue->sig = *pl;

    *pl = nue;

    return TODO_OK;
}

int ponerListaOrdenaAsc(tLista* pl, void* dato, size_t tam, int(*comparar)(const void*, const void*))
{
    tNodo* nue;
    int comp;

    while(*pl && (comp = comparar(dato, (*pl)->info)) > 0)
        pl = &(*pl)->sig;

    if(*pl || !comp)
        return DUPLICADO;

    nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
        return SIN_MEM;

    nue->info = malloc(tam);

    if(!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    nue->tamInfo = tam;
    memcpy(nue->info, dato, tam);
    nue->sig = *pl;

    *pl = nue;

    return TODO_OK;
}
int sacarListaOrdenaAsc(tLista* pl, void* dato, size_t tam, int(*comparar)(const void*, const void*))
{
    tNodo* nae;
    int comp;

    while(*pl && (comp = comparar(dato, (*pl)->info)) > 0)
        pl = &(*pl)->sig;

    if(!*pl || comp)
        return NO_EXISTE;

    nae = *pl;

    memcpy(dato, nae->info, MINIMO(tam, nae->tamInfo));

    *pl = nae->sig;

    free(nae->info);
    free(nae);

    return TODO_OK;
}
/**Adicionales**/
void ordenarSeleccionLista(tLista* pl, int(*comparar)(const void*, const void*))
{
    if(!*pl)
        return;

    while((*pl)->sig)
    {
        tNodo** menor = buscarMenorNodoLista(pl, comparar);

        if(pl != menor)
        {
            //intercambiarInfoNodos(pl, menor);
            reisentarNodoMenor(pl, menor);
        }

        pl = &(*pl)->sig;
    }
}

void intercambiarNodos(tNodo* n1, tNodo* n2)
{
    void *auxInfo;
    unsigned auxTam;

    auxInfo = n1->info;
    auxTam = n1->tamInfo;

    n1->info = n2->info;
    n1->tamInfo = n2->tamInfo;

    n2->info = auxInfo;
    n2->tamInfo = auxTam;

}

void intercambiarInfoNodos(tLista* pl, tLista* menor)
{
    tNodo* aux;

    aux = *pl;
    *pl = *menor;
    *menor = aux;
}
void reisentarNodoMenor(tLista* pl, tLista* menor)
{
    tNodo* aux = *menor;
    *menor = aux->sig;

    aux->sig = *pl;
    *pl = aux;
}

tNodo** buscarMenorNodoLista(const tLista* pl, int(*comparar)(const void*, const void*))
{
    const tLista* menor = pl;

    if(!*pl)
    {
        return NULL;
    }

    pl = &(*pl)->sig;

    while(*pl)
    {
        if(comparar((*pl)->info, (*menor)->info) < 0)
        {
            menor = pl;
        }

        pl = &(*pl)->sig;
    }

    return((tNodo**)menor);
}

void recorrerLista(tLista* pl, void(*accion)(void*, void*), void* param)
{
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}

void map(tLista* pl, void(*accion)(void*, void*), void* param)
{
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}
void filter(tLista* pl, int (*filtro)(void*, void*), void* param)
{
    while(*pl)
    {
        if(filtro((*pl)->info, param))
        {
            tNodo* elim = *pl;
            *pl = elim->sig;

            free(elim->info);
            free(elim);
        }
        else
        {
            pl = &(*pl)->sig;
        }
    }
}
void reduce(tLista* pl, void* result, void(*accion)(void*, void*, void*), void* param)
{
    while(*pl)
    {
        accion((*pl)->info, result, param);
        pl = &(*pl)->sig;
    }
}

void eliminarDuplicadosListaOrdenada(tLista* pl, int(*comparar)(const void*, const void*), void(*accion)(void*, const void*))
{
    tLista* aux;
    tNodo* pNae;

    while(*pl)
    {
        aux = &(*pl)->sig;

        while(*aux && !comparar((*pl)->info, (*aux)->info))
        {
            accion((*pl)->info, (*aux)->info);
            pNae = *aux;
            *aux = pNae->sig;

            free(pNae->info);
            free(pNae);
        }

        pl = &(*pl)->sig;
    }
}

int mostrarLista(tLista* pl, void(*mostrar)(const void*))
{
    tLista *pLista;

    if(*pl == NULL)
    {
        return LISTA_VACIA;
    }

    pLista = pl;

    while(*pLista != NULL)
    {
        mostrar((*pLista)->info); //mostrar dato
        pLista= &(*pLista)->sig;
    }

    return TODO_OK;
}
