#include "fecha.h"

static int esBisiesto(int anio)
{
    return (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
}

static int diasDelMes(int mes, int anio)
{
    static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return (mes == 2 && esBisiesto(anio)) ? 29 : *(dias + mes - 1);
}

/** <0 si f1 es anterior, 0 si son iguales, >0 si f1 es posterior **/
int cmpFechas(const t_fecha *f1, const t_fecha *f2)
{
    return (f1->anio - f2->anio) * 10000
         + (f1->mes  - f2->mes)  * 100
         + (f1->dia  - f2->dia);
}

int esFechaValida(const t_fecha *f)
{
    return f->anio >= 1 && f->mes >= 1 && f->mes <= 12
        && f->dia >= 1 && f->dia <= diasDelMes(f->mes, f->anio);
}
