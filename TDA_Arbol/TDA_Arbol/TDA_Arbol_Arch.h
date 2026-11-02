/// =======================================================================================
/// @file TDA_Arbol_Arch.h
/// @brief Funciones para interacci�n entre �rboles binarios y archivos (texto y binario).
///
/// Permite cargar �rboles desde archivos con datos ordenados o desordenados, y guardar
/// �rboles en archivos. Se trabaja tanto con archivos binarios como de texto, y se
/// pueden aplicar funciones de filtrado o trozado para manipular los datos.
/// =======================================================================================

#ifndef TDA_ARBOL_ARCH_H_INCLUDED
#define TDA_ARBOL_ARCH_H_INCLUDED

#include "../archivos_gen/archivos_gen.h"
#include "TDA_Arbol.h"

/// ==========================================================================
/// Cargar Archivo Desordenado a un Arbol
/// ==========================================================================

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

/// Carga arbol desde archivo binario abierto, con datos desordenados
int cargarDesdeArchBinDesArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tCmp cmpClave);
int cargarArchBinDesArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tCmp cmpClave);

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

/// Carga arbol desde archivo de texto abierto, con datos desordenados
int cargarDesdeArchTxtDesArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tCmp cmpClave, tTrozador leerTrozado);
int cargarArchTxtDesArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tCmp cmpClave, tTrozador leerTrozado);

/// ==========================================================================
/// Cargar Archivo Ordenado a un Arbol
/// ==========================================================================

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

/// Funci�n auxiliar recursiva para cargar �rbol balanceado desde binario
int _cargarDesdeArchBinOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo, int li, int ls);

/// Carga �rbol balanceado desde archivo binario abierto
int cargarDesdeArchBinOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo);

/// Igual que anterior, pero abriendo el archivo por nombre
int cargarArchBinOrdArbol(char *nombArch, tArbol *arbol, size_t tamInfo);

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

/// Funci�n auxiliar recursiva para cargar �rbol balanceado desde texto
int _cargarDesdeArchTxtOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tTrozador leerTrozado, int li, int ls);

/// Carga �rbol balanceado desde archivo de texto abierto
int cargarDesdeArchTxtOrdArbol(FILE *arch, tArbol *arbol , size_t tamInfo, tTrozador leerTrozado);

/// Igual que anterior, pero abriendo el archivo por nombre
int cargarArchTxtOrdArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tTrozador leerTrozado);

/// ==========================================================================
/// Creación de Archivos a Partir del Contenido de un Arbol
/// ==========================================================================

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

/// Crea archivo binario desde el contenido del �rbol (recorrido inorden)
void _crearDesdeArchBinArbol(void *info, size_t tamInfo, size_t nivel, void *param);
int crearDesdeArchBinArbol(FILE *arch, tArbol *arbol);
int crearArchBinArbol(char *nombArch, tArbol *arbol);

/// Crea archivo de texto desde �rbol, usando funci�n `crearTrozado`

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

typedef struct
{
    FILE* arch;
    tTrozador crearTrozado;
} tContextoArchTxt;

void _crearArchTxtArbol(void *info, size_t tamInfo, size_t nivel, void *param);
int crearArchTxtArbol(char *nombArch, tArbol *arbol, tTrozador crearTrozado);

#endif // TDA_ARBOL_ARCH_H_INCLUDED
