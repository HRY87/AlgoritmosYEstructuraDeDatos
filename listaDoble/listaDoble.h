#ifndef LISTADOBLE_H_INCLUDED
#define LISTADOBLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIN_MEM         -1
#define TODO_OK         0
#define LISTA_VACIA     1
#define DUPLICADO       2
#define NO_EXISTE       3

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig,
                    *ant;
}tNodoD;


typedef tNodoD  *tListaD;


void crearListaDoble(tListaD* pl);
int vaciarListaDoble(tListaD* pl);
int listaDobleVacia(const tListaD* pl);
int listaDobleLlena(const tListaD* pl, unsigned cantBytes);
int ponerAlInicioListaDoble(tListaD* pl, const void* dato, unsigned cantBytes);
int ponerAlFinalListaDoble(tListaD* pl, const void* dato, unsigned cantBytes);
int ponerEnOrdenListaDoble(tListaD* pl, const void* dato, unsigned cantBytes, int(*comparar)(const void*, const void*),
                           int(*acumular)(void**, unsigned*, const void*, unsigned));
void ordenarLista(tListaD *pl, int(*comparar)(const void*, const void*));
int mostrarDeIzqDerListaDoble(tListaD* pl, void(*mostrar)(const void*));
int mostrarDeDerIzqListaDoble(tListaD* pl, void(*mostrar)(const void*));
int eliminarPorClave(tListaD *pl, void* dato, unsigned cantBytes, int(*comparar)(const void*, const void*));
#endif // LISTADOBLE_H_INCLUDED
