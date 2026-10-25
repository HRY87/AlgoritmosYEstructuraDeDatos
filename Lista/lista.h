#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(X,Y)     ((X)<(Y)?(X):(Y))

#define SIN_MEM                 -1
#define TODO_OK                  0
#define DUPLICADO                1
#define LISTA_VACIA              2
#define LISTA_LLENA              3
#define NO_EXISTE                4

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig;
}tNodo;

typedef tNodo   *tLista;

void crearLista(tLista *pl);
int listaVacia(const tLista *pl);
int listaLlena(const tLista *pl, unsigned cantBytes);
void vaciarLista(tLista *pl);

int ponerAlComienzoLista(tLista *pl, const void* dato, unsigned cantBytes);
int sacarPrimeroLista(tLista *pl, void* dato, unsigned cantBytes);
int verPrimeroLista(const tLista *pl, void* dato, unsigned cantBytes);

int ponerAlFinalLista(tLista* pl, const void* dato, unsigned cantBytes);
int sacarUltimoLista(tLista *pl, void* dato, unsigned cantBytes);
int verUltimoLista(const tLista *pl, void* dato, unsigned cantBytes);

int ponerPosicionLista(tLista* pl, void* dato, size_t tam, unsigned pos);
int ponerListaOrdenaAsc(tLista* pl, void* dato, size_t tam, int(*comparar)(const void*, const void*));
int sacarListaOrdenaAsc(tLista* pl, void* dato, size_t tam, int(*comparar)(const void*, const void*));

/**Adicionales**/
void ordenarSeleccionLista(tLista* pl, int(*comparar)(const void*, const void*));
void intercambiarNodos(tNodo* n1, tNodo* n2);
void reisentarNodoMenor(tLista* pl, tLista* menor);
tNodo** buscarMenorNodoLista(const tLista* pl, int(*comparar)(const void*, const void*));

void recorrerLista(tLista* pl, void(*accion)(void*, void*), void* param);

void map(tLista* pl, void(*accion)(void*, void*), void* param);
void filter(tLista* pl, int (*filtro)(void*, void*), void* param);
void reduce(tLista* pl, void* result, void(*accion)(void*, void*, void*), void* param);

void eliminarDuplicadosListaOrdenada(tLista* pl, int(*comparar)(const void*, const void*), void(*accion)(void*, const void*));

int mostrarLista(tLista* pl, void(*mostrar)(const void*));

#endif // LISTA_H_INCLUDED
