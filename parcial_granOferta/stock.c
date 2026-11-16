#include <stdio.h>
#include "stock.h"
#include "comun.h"

/** Ordenado por codProd. Pensado para fecha actual = 15/11/2026 **/
int generarLotePrueba(const char *nomArch)
{
    static const tStock lote[] = {
        {"A00001", "Yerba mate 500g",     101, {20, 11, 2026},  50},
        {"A00002", "Fideos tirabuzon",    102, {10, 11, 2026},  30}, /* vencido */
        {"B00001", "Aceite girasol 1l",   201, {25, 12, 2026},  40},
        {"B00002", "Leche entera 1l",     202, {18, 11, 2026}, 120},
        {"C00001", "Galletitas dulces",   301, {18, 11, 2026},  80},
        {"C00002", "Mermelada durazno",   302, {20, 11, 2026},  15},
        {"D00001", "Arroz largo fino",    401, {18, 11, 2026}, 200},
        {"D00002", "Azucar 1kg",          402, { 5,  1, 2027},  60},
        {"E00001", "Cafe molido",         501, {20, 11, 2026},  25},
        {"E00002", "Te en saquitos",      502, {30,  6, 2027},  70},
        {"F00001", "Jugo en polvo",       601, {16, 11, 2026},  90},
        {"F00002", "Harina 000 1kg",      602, { 1,  1, 2026},  10}, /* vencido */
        {"G00001", "Pan lactal",          701, {15, 11, 2026},  35}  /* vence hoy */
    };
    FILE *pf = fopen(nomArch, "wb");
    size_t escritos;

    if(!pf)
        return ERROR_ARCH;

    escritos = fwrite(lote, sizeof(tStock), CANT_ELEM(lote), pf);
    fclose(pf);

    return escritos == CANT_ELEM(lote) ? TODO_OK : ERROR_ARCH;
}
