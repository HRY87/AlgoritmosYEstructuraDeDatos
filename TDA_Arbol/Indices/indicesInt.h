/// =======================================================================================
/// @file TDA_Indice_Int.h
/// @brief M�dulo para manejo de archivos �ndice con claves enteras (`int`).
///
/// Este m�dulo permite crear, buscar, modificar y recorrer archivos utilizando un �ndice
/// basado en claves de tipo entero. Es compatible con archivos binarios y de texto.
/// =======================================================================================

#ifndef TDA_ARBOL_IND_INT_H_INCLUDED
#define TDA_ARBOL_IND_INT_H_INCLUDED

#include "../TDA_Arbol/TDA_Arbol_Arch.h"

/// ==========================================================================
/// Estructuras auxiliares
/// ==========================================================================

typedef struct {
    int clave;   /// <-- Clave de b�squeda
    size_t pos;  /// <-- Posici�n del registro en el archivo original
} tIndiceInt;

typedef void (*tExtraer)(void *info, void *destClave);

/// ==========================================================================
/// B�squeda en archivo �ndice
/// ==========================================================================

/// B�squeda binaria en archivo �ndice abierto por clave entera
int busqBinDesdeArchIndiceInt(FILE *archInd, int clave, size_t *pos);

/// B�squeda binaria en archivo �ndice por nombre de archivo
int busqBinArchIndiceInt(char *nombArchInd, int clave, size_t *pos);

/// ==========================================================================
/// Archivos Binarios
/// ==========================================================================

/// Crea un archivo �ndice desde archivo binario abierto
int crearDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo,
                               tCmp cmpClave, tExtraer extraerClave,
                               tFilter filter, void *param);
/// Igual que el anterior, pero abre archivos por nombre
int crearArchBinIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                          tCmp cmpClave, tExtraer extraerClave,
                          tFilter filter, void *param);

/// Aplica una acci�n a cada registro del archivo original seg�n el �ndice
int recorrerDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo,
                             tAccion accion, void *param);
/// Versi�n por nombre del anterior
int recorrerArchBinIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                        tAccion accion, void *param);

/// Busca un elemento por clave usando el �ndice y lo guarda en destino
int busqDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd,
                              int clave, void *destino, size_t tamDes);
/// Igual que anterior pero con archivos abiertos por nombre
int busqArchBinIndiceInt(char *nombArchOri, char *nombArchInd,
                         int clave, void *destino, size_t tamDes);

/// Modifica un registro en archivo original usando el �ndice
int modDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd,
                             int clave, void *info, size_t tamInfo);
/// Igual que anterior, usando nombres de archivo
int modArchBinIndiceInt(char *nombArchOri, char *nombArchInd,
                        int clave, void *info, size_t tamInfo);

/// ==========================================================================
/// Archivos Texto
/// ==========================================================================

/// Crea un archivo �ndice desde un archivo de texto abierto
int crearDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo,
                               tCmp cmpClave, tAccion accion,
                               tTrozador leerTrozado,
                               tFilter filter, void *param);
/// Igual que anterior pero con nombres de archivo
int crearArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                          tCmp cmpClave, tAccion accion,
                          tTrozador leerTrozado,
                          tFilter filter, void *param);

/// Aplica acci�n a cada registro usando �ndice en archivo de texto abierto
int recorrerDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo,
                             tAccion accion, void *param,
                             tTrozador leerTrozado);
/// Igual que anterior, usando nombres de archivo
int recorrerArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo,
                        tAccion accion, void *param,
                        tTrozador leerTrozado);

/// Busca un registro por clave en archivo de texto abierto
int busqDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd,
                              int clave, void *destino,
                              tTrozador leerTrozado);
/// Igual que anterior pero con nombres de archivo
int busqArchTxtIndiceInt(char *nombArchOri, char *nombArchInd,
                         int clave, void *destino,
                         tTrozador leerTrozado);

/// Modifica un registro en archivo de texto usando el �ndice
int modArchTxtIndiceInt(char *nombArchOri, char *nombArchInd,
                        int clave, void *nuevo, size_t tamInfo,
                        tTrozador leerTrozado, tTrozador crearTrozado);

#endif // TDA_ARBOL_IND_INT_H_INCLUDED
