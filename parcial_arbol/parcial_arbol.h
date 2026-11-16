#ifndef PARCIALARBOL_H_INCLUDED
#define PARCIALARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "arbolBusqueda.h"

/// Largo maximo de linea (se puede cambiar en compilacion con -DMAXLINEA=n)
#ifndef MAXLINEA
#define MAXLINEA 10
#endif

#define MAX_PALABRA 64      /// Largo maximo de una palabra (el resto se descarta)

#ifndef MINIMO
#define MINIMO(X,Y) ((X)<(Y)?(X):(Y))
#endif

/// Palabra "en construccion"; puede quedar pendiente entre dos lineas
typedef struct
{
    char pal[MAX_PALABRA + 1];
    char *fin;                  /// Proxima posicion libre de pal
} tAcumPal;

/// Contexto para recorrer A y buscar cada palabra en B
typedef struct
{
    tArbolBinBusq *arbolB;
    FILE *arch;
    unsigned cant;
} tCtxDif;

int cmpPalabra(const void *a, const void *b);
void vaciarArbolBinBusq(tArbolBinBusq *p);

/// Carga desde archivo de texto
int _volcarPalabra(tArbolBinBusq *arbol, tAcumPal *acum);
int _procesarLinea(const char *linea, size_t lon, tArbolBinBusq *arbol, tAcumPal *acum);
int cargarDesdeArchTxtPalabrasArbol(FILE *arch, tArbolBinBusq *arbol);
int cargarArchTxtPalabrasArbol(const char *nombArch, tArbolBinBusq *arbol);

/// Diferencia A - B
void _difArbol(void *info, unsigned tam, unsigned nivel, void *param);
int difDesdeArchTxtArbol(tArbolBinBusq *a, tArbolBinBusq *b, FILE *arch);
int difArchTxtArbol(tArbolBinBusq *a, tArbolBinBusq *b, const char *nombArchSal);

#endif // PARCIALARBOL_H_INCLUDED
