/// =======================================================================================
/// @file TDA_Indice_Str.h
/// @brief M�dulo para manejo de archivos �ndice con claves tipo string.
///
/// Este m�dulo permite crear, buscar, recorrer y modificar archivos utilizando
/// �ndices basados en claves de texto (`char[]`).
/// =======================================================================================

#ifndef TDA_ARBOL_IND_STR_H_INCLUDED
#define TDA_ARBOL_IND_STR_H_INCLUDED

#include "../TDA_Arbol/TDA_Arbol_Arch.h"

#define TAM_CLAVE 12  /// Tama�o fijo de la clave string

/// ==========================================================================
/// Estructuras auxiliares
/// ==========================================================================


typedef struct {
    char clave[TAM_CLAVE];  ///<-- Clave textual asociada al registro
    size_t pos;             ///<-- Posici�n del registro en el archivo original
} tIndiceStr;

typedef void (*tExtraer)(void *info, void *destClave);

/// ==========================================================================
/// B�squeda con �ndice
/// ==========================================================================

/// B�squeda binaria en �ndice abierto
int busqBinDesdeArchIndiceStr(FILE *archInd, const char *clave, size_t *pos);
/// B�squeda binaria en �ndice por nombre de archivo
int busqBinArchIndiceStr(char *nombArchInd, const char *clave, size_t *pos);

/// ==========================================================================
/// Archivos Binarios
/// ==========================================================================

/// Crear archivo �ndice desde archivo binario abierto
int crearDesdeArchBinIndiceStr(FILE *archOri, FILE *archInd, size_t tamInfo,
                               tCmp cmpClave, tExtraer extraerClave,
                               tFilter filter, void *param);
/// Igual que anterior, pero con nombres de archivo
int crearArchBinIndiceStr(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                          tCmp cmpClave, tExtraer extraerClave,
                          tFilter filter, void *param);

/// Aplicar funci�n a cada elemento seg�n �ndice (archivos abiertos)
int recorrerDesdeArchBinIndiceStr(FILE *archOri, FILE *archInd, size_t tamInfo,
                             tAccion accion, void *param);
/// Igual que anterior pero con nombres de archivo
int recorrerArchBinIndiceStr(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                        tAccion accion, void *param);

/// Buscar un elemento por clave y cargarlo desde archivo binario abierto
int busqDesdeArchBinIndiceStr(FILE *archOri, FILE *archInd,
                              const char *clave, void *destino, size_t tamDes);
/// Igual que anterior, pero por nombre de archivo
int busqArchBinIndiceStr(char *nombArchOri, char *nombArchInd,
                         const char *clave, void *destino, size_t tamDes);

/// Modificar un elemento en archivo binario (archivos abiertos)
int modDesdeArchBinIndiceStr(FILE *archOri, FILE *archInd,
                             const char *clave, void *info, size_t tamInfo);

/// Igual que anterior, pero con nombres de archivo
int modArchBinIndiceStr(char *nombArchOri, char *nombArchInd,
                        const char *clave, void *info, size_t tamInfo);

/// ==========================================================================
/// Archivos Texto
/// ==========================================================================

/// Crear archivo �ndice desde archivo de texto abierto
int crearDesdeArchTxtIndiceStr(FILE *archOri, FILE *archInd, size_t tamInfo,
                               tCmp cmpClave, tAccion accion,
                               tTrozador leerTrozado,
                               tFilter filter, void *param);
/// Igual que anterior pero con nombres de archivo
int crearArchTxtIndiceStr(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                          tCmp cmpClave, tAccion accion,
                          tTrozador leerTrozado,
                          tFilter filter, void *param);

/// Recorrer archivo texto usando �ndice (archivos abiertos)
int recorrerDesdeArchTxtIndiceStr(FILE *archOri, FILE *archInd, size_t tamInfo,
                             tAccion accion, void *param,
                             tTrozador leerTrozado);
/// Igual que anterior pero con nombres de archivo
int recorrerArchTxtIndiceStr(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                        tAccion accion, void *param,
                        tTrozador leerTrozado);

/// Buscar un elemento en archivo texto (archivos abiertos)
int busqDesdeArchTxtIndiceStr(FILE *archOri, FILE *archInd,
                              const char *clave, void *destino,
                              tTrozador leerTrozado);
/// Igual que anterior pero con nombres de archivo
int busqArchTxtIndiceStr(char *nombArchOri, char *nombArchInd,
                         const char *clave, void *destino,
                         tTrozador leerTrozado);

/// Modificar un elemento en archivo texto (usando �ndice)
int modArchTxtIndiceStr(char *nombArchOri, char *nombArchInd,
                        const char *clave, void *nuevo, size_t tamInfo,
                        tTrozador leerTrozado, tTrozador crearTrozado);

#endif // TDA_ARBOL_IND_STR_H_INCLUDED
