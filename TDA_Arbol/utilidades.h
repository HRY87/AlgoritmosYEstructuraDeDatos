/// =======================================================================================
/// @file utilidades.h
/// @brief Funciones auxiliares para pruebas, comparaciones, filters y visualizaci�n.
///
/// Este m�dulo contiene funciones de comparaci�n, recorrereo, filtrado, y visualizaci�n para
/// tipos de datos gen�ricos como `int` y estructuras como `tPrueba`, usadas en el manejo
/// de �rboles y archivos con �ndice.
/// =======================================================================================

#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

#include "Indices/indices.h"
#include "Indices/indices.h"

/// ==================================================================
/// Funciones Int
/// ==================================================================

void mostrarInt(void *info, void *param);

int cmpInt(const void *a, const void *b);

/// ==================================================================
/// Funciones de Estructuras de Prueba
/// ==================================================================

#define TAM_NOM     30
#define TAM_CARRERA 20

typedef struct {
    int legajo;
    char nombre[TAM_NOM];
    char carrera[TAM_CARRERA];
    float promedio;
    char estado;
} tAlu;

int generarLoteAlu(char *nombArch);

void mostrarAluCabe();
void mostrarAlu(void *info, void *param);
void bajaAlu(void *info, void *param);

int cmpAlu(const void *a, const void *b);
void extraerClaveAlu(void *info, void *destClave);

int filtroEstAlu(void *info, void *param);

#endif // UTILIDADES_H_INCLUDED
