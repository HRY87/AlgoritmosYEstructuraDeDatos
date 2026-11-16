#ifndef LOTEPRUEBA_H_INCLUDED
#define LOTEPRUEBA_H_INCLUDED

#include "parcial_arbol.h"

/// Escribe 'texto' (lineas separadas por '\n') cortando cada linea cada MAXLINEA caracteres
int generarArchTxtCortado(const char *nombArch, const char *texto);

/// Genera A.TXT y B.TXT de prueba
int generarLote(const char *nombA, const char *nombB);

#endif // LOTEPRUEBA_H_INCLUDED
