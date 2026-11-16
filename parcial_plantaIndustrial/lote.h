#ifndef LOTE_H_INCLUDED
#define LOTE_H_INCLUDED

#include "lista.h"   /* codigos de retorno SIN_MEM y TODO_OK */

#define ERROR_ARCH      -2

#define TAM_COD_LOTE    11
#define TAM_RESULTADO   6
#define RES_OK          "OK"
#define RES_FALLA       "FALLA"

#define ARCH_ENTRADA    "entrada.bin"

typedef struct
{
    char    codLote[TAM_COD_LOTE];
    int     idProducto;
    char    resultadoControl[TAM_RESULTADO];
}tRegistro;

int generarLoteDePrueba(const char *path);
int procesarEntrada(const char *pathEntrada, int x);
int mostrarEntrada(const char *pathEntrada);
int mostrarSalidas(int x);

#endif // LOTE_H_INCLUDED
