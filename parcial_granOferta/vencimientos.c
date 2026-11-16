#include <stdio.h>
#include <string.h>
#include "vencimientos.h"

typedef struct
{
    FILE     *pf;
    unsigned orden;
}tParamGrabar;

/** Primitivas genericas (puntero a funcion) **/
static int cmpProdVto(const void *a, const void *b)
{
    return cmpFechas(&((const tProdVto*)a)->fVto, &((const tProdVto*)b)->fVto);
}

static void mostrarProdOrden(void *info, void *param)
{
    const tProdVto *prod = (const tProdVto*)info;
    unsigned *orden = (unsigned*)param;

    printf("%5u  %-6s  %02d/%02d/%04d  %10u\n", ++(*orden), prod->codProd,
           prod->fVto.dia, prod->fVto.mes, prod->fVto.anio, prod->cant);
}

static void grabarProdOrden(void *info, void *param)
{
    const tProdVto *prod = (const tProdVto*)info;
    tParamGrabar *pg = (tParamGrabar*)param;

    fprintf(pg->pf, "%04u%-6.6s%02d/%02d/%04d%010u\n", ++pg->orden, prod->codProd,
            prod->fVto.dia, prod->fVto.mes, prod->fVto.anio, prod->cant);
}

/** Si hay empate de fecha, se queda con el de mayor cantidad **/
static void elegirMasProximo(void *info, void *result, void *param)
{
    const tProdVto *prod = (const tProdVto*)info;
    tProdVto *mejor = (tProdVto*)result;
    int cmp = cmpFechas(&prod->fVto, &mejor->fVto);

    (void)param;

    if(cmp < 0 || (!cmp && prod->cant > mejor->cant))
        *mejor = *prod;
}

/** Auxiliares **/
static void reducirStock(tProdVto *prod, const tStock *stock)
{
    strcpy(prod->codProd, stock->codProd);
    prod->fVto = stock->fVto;
    prod->cant = stock->cant;
}

/** Si ya hay n elementos, solo entra si no vence despues del ultimo **/
static int entraEnLosProximos(const tLista *pl, const tProdVto *prod, unsigned cant, unsigned n)
{
    tProdVto ultimo;

    return cant < n
        || (verUltimoLista(pl, &ultimo, sizeof(ultimo)) == TODO_OK
            && cmpProdVto(prod, &ultimo) <= 0);
}

static int agregarSiCorresponde(tLista *pl, const tProdVto *prod, unsigned *cant, unsigned n)
{
    int ret = TODO_OK;

    if(entraEnLosProximos(pl, prod, *cant, n))
    {
        ret = ponerListaOrdenadaDup(pl, prod, sizeof(tProdVto), cmpProdVto);

        if(ret == TODO_OK)
        {
            (*cant)++;

            if(*cant > n)
                *cant -= recortarListaMayoresAlNEsimo(pl, n, cmpProdVto);
        }
    }

    return ret;
}

/** Primitivas del parcial **/
int cargarProximosAVencer(const char *nomArch, const t_fecha *hoy, unsigned n, tLista *pl)
{
    FILE *pf = fopen(nomArch, "rb");
    tStock stock;
    tProdVto prod;
    unsigned cant = 0;
    int ret = TODO_OK;

    if(!pf)
        return ERROR_ARCH;

    while(ret == TODO_OK && fread(&stock, sizeof(tStock), 1, pf) == 1)
    {
        if(cmpFechas(&stock.fVto, hoy) >= 0)
        {
            reducirStock(&prod, &stock);
            ret = agregarSiCorresponde(pl, &prod, &cant, n);
        }
    }

    fclose(pf);

    if(ret != TODO_OK)
        vaciarLista(pl);

    return ret;
}

int mostrarProximosAVencer(tLista *pl)
{
    unsigned orden = 0;

    if(listaVacia(pl))
        return LISTA_VACIA;

    printf("\n%5s  %-6s  %-10s  %10s\n", "Orden", "Codigo", "Vencimiento", "Cantidad");
    recorrerLista(pl, mostrarProdOrden, &orden);

    return TODO_OK;
}

int grabarProximosAVencer(tLista *pl, const char *nomArch)
{
    tParamGrabar pg;

    pg.pf = fopen(nomArch, "wt");

    if(!pg.pf)
        return ERROR_ARCH;

    pg.orden = 0;
    recorrerLista(pl, grabarProdOrden, &pg);
    fclose(pg.pf);

    return TODO_OK;
}

int obtenerMasProximoAVencer(tLista *pl, tProdVto *prod)
{
    static const tProdVto centinela = {"", {31, 12, 9999}, 0};

    if(listaVacia(pl))
        return LISTA_VACIA;

    *prod = centinela;
    reduce(pl, prod, elegirMasProximo, NULL);

    return TODO_OK;
}
