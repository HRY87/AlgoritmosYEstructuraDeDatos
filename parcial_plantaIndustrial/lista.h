#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define SIN_MEM     -1
#define TODO_OK      0

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig;
}tNodo;

typedef tNodo   *tLista;

void crearLista(tLista *pl);
int ponerEnOrdenLista(tLista *pl, const void *dato, unsigned cantBytes,
                      int(*comparar)(const void*, const void*));
void recorrerLista(tLista *pl, void(*accion)(void*, void*), void *param);
void vaciarLista(tLista *pl);

#endif // LISTA_H_INCLUDED
