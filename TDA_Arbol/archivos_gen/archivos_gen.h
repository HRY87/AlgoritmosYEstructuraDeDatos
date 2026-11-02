/// =======================================================================================
/// @file archivos_gen.h
/// @brief Funciones gen�ricas para trabajar con archivos binarios y de texto.
///
/// Provee operaciones para crear archivos a partir de vectores en memoria, leerlos
/// secuencialmente o por posici�n, y aplicar funciones a cada elemento.
/// =======================================================================================

#ifndef ARCHS_H_INCLUDED
#define ARCHS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archivos_gen.h"

/// =====================================================================================================
/// Macros
/// =====================================================================================================

#define ERR_ARCH -1   // Error al abrir archivo
#define POS_INV  -3   // Posici�n inv�lida
#define SIN_MEM  -2   // Falta de memoria
#define EXITO     1   // �xito

#define BUFFER   256  // Tama�o del buffer de texto

/// =====================================================================================================
/// Tipos Genericos
/// =====================================================================================================

/// Funci�n de acci�n para aplicar sobre un dato le�do del archivo
typedef void (*tAccion)(void *dato, void *param);

/// Funci�n de conversi�n para trozar o generar texto desde una estructura
typedef int (*tTrozador)(char *linea, void *dato);

/// =====================================================================================================
/// Archivos Binarios
/// =====================================================================================================

/// Crea un archivo binario desde un vector
int crearArchBin(char *nombArch, void *vec, size_t lon, size_t tamEl);

/// Lee un archivo binario abierto y aplica una acción
int leerDesdeArchBin(FILE *arch, size_t tamInfo , tAccion accion, void *param);
int leerArchBin(char *nombArch, size_t tamInfo , tAccion accion, void *param);

/// Escribir un registro en una posición determinada
int escribirPosDesdeArchBin(FILE *arch, void *dato, size_t tamDato, size_t pos);
int escribirPosArchBin(const char *nombArch, void *dato, size_t tamDato, size_t pos);

/// Lee un elemento en una posición
int leerPosDesdeArchBin(FILE *arch, void *destino, size_t tamDes, size_t pos);
int leerPosArchBin(char *nombArch, void *destino, size_t tamDes, size_t pos);

/// =====================================================================================================
/// Archivos Texto
/// =====================================================================================================

/// Crea un archivo de texto desde un vector (usando funci�n trozadora)
int crearArchTxt(char *nombArch, void *vec, size_t lon, size_t tamEl, tTrozador crearTrozado);

/// Lee un archivo texto abierto y aplica una acción
int leerDesdeArchTxt(FILE *arch, size_t tamInfo, tTrozador leerTrozado, tAccion accion, void *param);
int leerArchTxt(char *nombArch, size_t tamInfo, tTrozador leerTrozado, tAccion accion, void *param);

/// Lee un elemento en una posición
int leerPosDesdeArchTxt(FILE *arch, void *destino, size_t tamDes, size_t pos, tTrozador leerTrozado);
int leerPosArchTxt(char *nombArch, void *destino, size_t tamDes, size_t pos, tTrozador leerTrozado);

#endif // ARCHS_H_INCLUDED
