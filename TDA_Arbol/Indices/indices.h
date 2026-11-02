/// =======================================================================================
/// @file indices.h
/// @brief Modulo para manejo de archivos indice con claves enteras (`int`).
///
/// Este modulo permite crear, buscar, modificar y recorrer archivos binarios utilizando un indice
/// basado en claves de tipo entero. Es compatible solo con archivos binarios.
/// =======================================================================================

#ifndef INDICES_H_INCLUDED
#define INDICES_H_INCLUDED

#include <string.h>
#include "../TDA_Arbol/TDA_Arbol_Arch.h"

/// ==========================================================================
/// Macros
/// ==========================================================================

#define ERR_NO_INICIALIZADO -6
#define FORMATO_INDICE ".idx"
#define TAM_NOMB_ARCH 256

/// ==========================================================================
/// Estructuras auxiliares
/// ==========================================================================

/// Función que extrae la clave desde un registro (info) y la guarda en destClave
typedef void (*tExtraer)(void *info, void *destClave);
typedef int (*tCmp)(const void* a, const void* b);

/// Registro índice simple con clave entera y posición en archivo
typedef struct {
    int clave;  // Clave por la cual se desea buscar
    size_t pos; // Posición de la clave en el archivo original
} tInd;

/// Contexto general de índice con árbol en memoria y archivos asociados.
typedef struct {
    char nombArchOri[TAM_NOMB_ARCH]; // Nombre del archivo original
    char nombArchInd[TAM_NOMB_ARCH]; // Nombre del archivo indice
    FILE *archOri;                   // Archivo original de registros
    FILE *archInd;                   // Archivo índice
    tArbol arbol;                    // Árbol que actúa como índice
    size_t tamInfo;                  // Tamaño de cada registro de archOri
    tCmp cmpClave;                   // Cmp de claves de archOri
    tExtraer extraerClave;           // Extracción de clave de archOri
} tIndCtx;

/// Estructura usada para recorrer el árbol índice y aplicar una acción
typedef struct {
    tIndCtx *ctx;
    tAccion accion;
    void *param;
} tIndCtxReco;

/// Esctructura para recorrer el árbol índice y aplicar una acción segun lo que se encuentr
typedef struct {
    tIndCtx *ctx;
    tFilter filtroUsuario;
    void *param;
} tIndCtxFiltro;


/// ==========================================================================
/// Funciones Auxiliares
/// ==========================================================================

// Comparar 2 elementos del tipo tInd según la clave
int cmpIndice(const void *a, const void *b);

/// ==========================================================================
/// Funciones Principales
/// ==========================================================================

// Crea un archivo índice desde el archivo original (recorre y guarda claves)
int crearArchInd(char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave);

// Inicializa el contexto de índice: abre archivos y crea el árbol
int inicializarIndCtx(tIndCtx *ctx, char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave);

// Cierra archivos abiertos y limpia el árbol
int finalizarIndCtx(tIndCtx *ctx);

// Inserta un nuevo registro en el archivo original y actualiza el índice
int insArchInd(tIndCtx *ctx, void *info);

// Busca un registro según la clave, lo carga en info
int busqArchInd(tIndCtx *ctx, void *info);

// Modifica un registro existente con la clave dada
int modArchInd(tIndCtx *ctx, void *info, tAccion accion, void *param);

// Eliminación logica de un registro al eliminar el nodo
int elimArchInd(tIndCtx *ctx, void *info);

// Recorre el archivo original en orden (según el árbol índice) y aplica la acción
void _recorrerArchInd(void *info, size_t tamInfo, size_t nivel, void *param);
int recorrerArchInd(tIndCtx *ctx, tAccion accion, void *param);

// Filtra el árbol índice eliminando nodos cuyos registros del archivo original no cumplan el filtro.
int _filtrarArchInd(void *info, void *param);
int filtrarArchInd(tIndCtx *ctx, tFilter filtroUsuario, void *param);

/// ==========================================================================
/// Utilidades
/// ==========================================================================

void mostrarIndCabe();                    // Mostrar cabecera de la estructura
void mostrarInd(void *info, void *param); // Mostrar estructura �ndice

#endif // INDICES_H_INCLUDED
