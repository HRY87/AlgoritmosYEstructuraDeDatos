#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "comun.h"

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig;
}tNodo;

typedef tNodo   *tLista;

void crearLista(tLista *pl);
int listaVacia(const tLista *pl);
void vaciarLista(tLista *pl);

int ponerListaOrdenadaDup(tLista *pl, const void *dato, unsigned cantBytes,
                          int (*comparar)(const void*, const void*));
int verUltimoLista(const tLista *pl, void *dato, unsigned cantBytes);
unsigned recortarListaMayoresAlNEsimo(tLista *pl, unsigned n,
                                      int (*comparar)(const void*, const void*));

void recorrerLista(tLista *pl, void (*accion)(void*, void*), void *param);
void reduce(tLista *pl, void *result, void (*accion)(void*, void*, void*), void *param);

#endif // LISTA_H_INCLUDED
