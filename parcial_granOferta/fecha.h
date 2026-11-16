#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

typedef struct
{
    int dia,
        mes,
        anio;
}t_fecha;

int cmpFechas(const t_fecha *f1, const t_fecha *f2);
int esFechaValida(const t_fecha *f);

#endif // FECHA_H_INCLUDED
