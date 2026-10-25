#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

#define CANT_ELEM(v)    (sizeof(v) / sizeof(*(v)))

/**Genericos**/
void mostrarInt(const void *d);
int  cmpInt(const void *a, const void *b);
void multiplicarInt(void *info, void *param);
int  esPar(void *info, void *param);
void sumarInt(void *res, void *info, void *prm);
void avisarDup(void *conservado, const void *eliminado);

/**Auxiliares**/
void imprimir(const char *titulo, tLista *pl);
void cargarAlFinal(tLista *pl, int *v, unsigned cant);
void cargarOrdenado(tLista *pl, int *v, unsigned cant);

/**Pruebas**/
void probarBasicas(void);
void probarOrdenamiento(void);
void probarFuncionalidad(void);

int main()
{
    probarBasicas();
    probarOrdenamiento();
    probarFuncionalidad();
    return 0;
}

/**Genericos**/
void mostrarInt(const void *d)
{
    printf("%d ", *(const int*)d);
}

int cmpInt(const void *a, const void *b)
{
    return *(const int*)a - *(const int*)b;
}

void multiplicarInt(void *info, void *param)
{
    *(int*)info *= *(int*)param;
}

int esPar(void *info, void *param)
{
    (void)param;
    return *(int*)info % 2 == 0;
}

void sumarInt(void *info, void *res, void *prm)
{
    (void)prm;
    *(int*)res += *(int*)info;
}

void avisarDup(void *conservado, const void *eliminado)
{
    (void)conservado;
    printf("[dup %d] ", *(const int*)eliminado);
}

/**Auxiliares**/
void imprimir(const char *titulo, tLista *pl)
{
    printf("%-28s: ", titulo);
    if(mostrarLista(pl, mostrarInt) == LISTA_VACIA)
        printf("(vacia)");
    printf("\n");
}

void cargarAlFinal(tLista *pl, int *v, unsigned cant)
{
    int *fin = v + cant;

    for(; v < fin; v++)
        ponerAlFinalLista(pl, v, sizeof(int));
}

void cargarOrdenado(tLista *pl, int *v, unsigned cant)
{
    int *fin = v + cant;

    for(; v < fin; v++)
        ponerListaOrdenaAsc(pl, v, sizeof(int), cmpInt);
}

/**Pruebas**/
void probarBasicas(void)
{
    tLista l;
    int d, a = 10, b = 20, c = 30, x = 15;

    printf("\n== Basicas ==\n");
    crearLista(&l);
    printf("listaVacia (esperado 1)     : %d\n", listaVacia(&l));
    printf("sacarPrimero en vacia       : %d (esperado %d)\n",
           sacarPrimeroLista(&l, &d, sizeof(int)), LISTA_VACIA);

    ponerAlComienzoLista(&l, &b, sizeof(int));
    ponerAlComienzoLista(&l, &a, sizeof(int));
    ponerAlFinalLista(&l, &c, sizeof(int));
    imprimir("Comienzo(20),Comienzo(10),Final(30)", &l);

    verPrimeroLista(&l, &d, sizeof(int));
    printf("verPrimero                  : %d (esperado 10)\n", d);
    verUltimoLista(&l, &d, sizeof(int));
    printf("verUltimo                   : %d (esperado 30)\n", d);

    printf("ponerPosicion(15, pos 1)    : %d\n",
           ponerPosicionLista(&l, &x, sizeof(int), 1));
    imprimir("Tras ponerPosicion", &l);

    sacarPrimeroLista(&l, &d, sizeof(int));
    printf("sacarPrimero                : %d\n", d);
    sacarUltimoLista(&l, &d, sizeof(int));
    printf("sacarUltimo                 : %d\n", d);
    imprimir("Tras sacar", &l);

    vaciarLista(&l);
    printf("Tras vaciar, listaVacia     : %d\n", listaVacia(&l));
}

void probarOrdenamiento(void)
{
    tLista l;
    int v[] = {50, 10, 40, 20, 30};
    int w[] = {30, 10, 20, 10, 20};
    int d[] = {10, 10, 20, 20, 20, 30};
    int inexistente = 99, existente = 20;

    printf("\n== Ordenamiento ==\n");
    crearLista(&l);
    cargarAlFinal(&l, v, CANT_ELEM(v));
    imprimir("Original", &l);
    ordenarSeleccionLista(&l, cmpInt);
    imprimir("ordenarSeleccionLista", &l);
    vaciarLista(&l);

    cargarOrdenado(&l, w, CANT_ELEM(w));
    imprimir("ponerOrdenado (dup ignorados)", &l);   /* 10 20 30 */

    printf("sacarOrdenado(20)           : %d (esperado %d)\n",
           sacarListaOrdenaAsc(&l, &existente, sizeof(int), cmpInt), TODO_OK);
    printf("sacarOrdenado(99)           : %d (esperado %d)\n",
           sacarListaOrdenaAsc(&l, &inexistente, sizeof(int), cmpInt), NO_EXISTE);
    imprimir("Tras sacarOrdenado", &l);               /* 10 30 */
    vaciarLista(&l);

    cargarAlFinal(&l, d, CANT_ELEM(d));
    imprimir("Con duplicados", &l);
    eliminarDuplicadosListaOrdenada(&l, cmpInt, avisarDup);
    printf("\n");
    imprimir("eliminarDuplicados", &l);               /* 10 20 30 */
    vaciarLista(&l);
}

void probarFuncionalidad(void)
{
    tLista l;
    int v[] = {1, 2, 3, 4, 5, 6};
    int factor = 10, suma = 0;

    printf("\n== filter / map / reduce / recorrer ==\n");
    crearLista(&l);
    cargarAlFinal(&l, v, CANT_ELEM(v));
    imprimir("Original", &l);

    filter(&l, esPar, NULL);
    imprimir("filter (elimina pares)", &l);           /* 1 3 5 */

    map(&l, multiplicarInt, &factor);
    imprimir("map (x10)", &l);                        /* 10 30 50 */

    reduce(&l, &suma, sumarInt, NULL);
    printf("reduce (suma)               : %d (esperado 90)\n", suma);

    recorrerLista(&l, multiplicarInt, &factor);
    imprimir("recorrerLista (x10)", &l);              /* 100 300 500 */

    vaciarLista(&l);
}
