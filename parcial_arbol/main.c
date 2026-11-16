#include <stdio.h>
#include "parcial_arbol.h"
#include "lotePrueba.h"

#define ARCH_A   "A.TXT"
#define ARCH_B   "B.TXT"
#define ARCH_SAL "AmenosB.txt"

int main()
{
    tArbolBinBusq arbolA, arbolB;
    int res;

    crearArbolBinBusq(&arbolA);
    crearArbolBinBusq(&arbolB);

    if((res = generarLote(ARCH_A, ARCH_B)) == TODO_BIEN
       && (res = cargarArchTxtPalabrasArbol(ARCH_A, &arbolA)) == TODO_BIEN
       && (res = cargarArchTxtPalabrasArbol(ARCH_B, &arbolB)) == TODO_BIEN)
    {
        res = difArchTxtArbol(&arbolA, &arbolB, ARCH_SAL);
    }

    if(res != TODO_BIEN)
        fprintf(stderr, "Error al procesar los archivos (sin memoria o archivo inexistente)\n");

    // Sin basura en memoria
    vaciarArbolBinBusq(&arbolA);
    vaciarArbolBinBusq(&arbolB);

    return res == TODO_BIEN ? 0 : 1;
}
