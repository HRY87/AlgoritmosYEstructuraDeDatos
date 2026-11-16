#ifndef STOCK_H_INCLUDED
#define STOCK_H_INCLUDED

#include "fecha.h"

#define TAM_COD     7   /* 6 caracteres + '\0' */
#define TAM_DESC    26  /* 25 caracteres + '\0' */

/** Registro completo de stock.dat **/
typedef struct
{
    char        codProd[TAM_COD];
    char        descrip[TAM_DESC];
    int         lote;
    t_fecha     fVto;
    unsigned    cant;
}tStock;

/** Lo unico que necesito guardar en memoria (sin descrip ni lote) **/
typedef struct
{
    char        codProd[TAM_COD];
    t_fecha     fVto;
    unsigned    cant;
}tProdVto;

int generarLotePrueba(const char *nomArch);

#endif // STOCK_H_INCLUDED
