#ifndef VENCIMIENTOS_H_INCLUDED
#define VENCIMIENTOS_H_INCLUDED

#include "lista.h"
#include "stock.h"

int cargarProximosAVencer(const char *nomArch, const t_fecha *hoy, unsigned n, tLista *pl);
int mostrarProximosAVencer(tLista *pl);
int grabarProximosAVencer(tLista *pl, const char *nomArch);
int obtenerMasProximoAVencer(tLista *pl, tProdVto *prod);

#endif // VENCIMIENTOS_H_INCLUDED
