/// =======================================================================================
/// @file arbol.h
/// @brief Definici�n y operaciones sobre �rboles Binarios Generales en C.
///
/// Este archivo define un TDA �rbol Binario con m�ltiples variantes de inserci�n,
/// recorridos, operaciones de b�squeda, conteo, an�lisis estructural (AVL, balanceado, completo), y eliminaci�n de nodos.
/// =======================================================================================

#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// C�digos de retorno
#define EXITO            1
#define SIN_MEM         -4
#define DUPLICADO       -3
#define NO_ES_HOJA      -2
#define NO_ENCONTRADO   -1
#define ERR_INSER       -6
#define ARBOL_VACIO      0
#define ELIM             7

#define MIN(X,Y) ((X) > (Y) ? (Y) : (X))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

/// Estructura del nodo del �rbol
typedef struct sNodo {
    void* info;
    unsigned tamInfo;
    struct sNodo* izq;
    struct sNodo* der;
} tNodo;

typedef tNodo* tArbol;

/// Tipos de funciones auxiliares
typedef int (*tCmp)(const void* a, const void* b);
typedef int (*tFilter)(void *info, void *param);
typedef void (*tAccion)(void *info, void *param);
typedef void (*tAccionRec)(void *info, size_t tamInfo, size_t nivel, void *param);
typedef void (*procesar)(const void* a);

/// ------------------------------------------------------------
/// Inicializaci�n
/// ------------------------------------------------------------
void crearArbol(tArbol* arbol); // Inicializa el �rbol como vac�o

/// ------------------------------------------------------------
/// Insertar
/// ------------------------------------------------------------
int insArb(tArbol *arbol, void *info, size_t tamInfo, tCmp cmpClave);      // Inserci�n est�ndar
int rinsArb(tArbol *arbol, void *info, size_t tamInfo, tCmp cmpClave);     // Inserci�n recursiva
int insDirArb(tArbol *arbol, void *info, size_t tamInfo, tCmp cmpClave);   // Inserci�n directa (sin verificar)

/// ------------------------------------------------------------
/// Recorridos
/// ------------------------------------------------------------

// Recorrido sentido normal
void recorrerPre(tArbol *arbol, tAccionRec accion, void *param);       // Pre-orden
void _recorrerPre(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

void recorrerIn(tArbol *arbol, tAccionRec accion, void *param);        // In-orden
void _recorrerIn(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

void recorrerPos(tArbol *arbol, tAccionRec accion, void *param);       // Pos-orden
void _recorrerPos(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

// Recorrido sentido invertido
void recorrerPreInv(tArbol *arbol, tAccionRec accion, void *param);    // Pre-orden inverso
void _recorrerPreInv(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

void recorrerInInv(tArbol *arbol, tAccionRec accion, void *param);     // In-orden inverso
void _recorrerInInv(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

void recorrerPosInv(tArbol *arbol, tAccionRec accion, void *param);    // Pos-orden inverso
void _recorrerPosInv(tArbol *arbol, tAccionRec accion, void *param, size_t nivel);

/// ------------------------------------------------------------
/// Estad�sticas del �rbol
/// ------------------------------------------------------------
size_t alturaArbol(tArbol *arbol);                        // Altura del �rbol
int esCompletoHastaNivelArbol(tArbol *arbol, size_t n);   // �Completo hasta nivel n?
int esCompletoArbol(tArbol *arbol);                       // �Es completo?
int esAVLArbol(tArbol *arbol);                            // �Cumple propiedad AVL?

/// ------------------------------------------------------------
/// Estad�sticas de los Nodos
/// ------------------------------------------------------------
size_t cantNodosArbol(tArbol *arbol);                     // Total de nodos
size_t cantHojasArbol(tArbol* arbol);                     // Nodos hoja
size_t cantNoHojasArbol(tArbol* arbol);                   // Nodos no hoja

size_t cantNodosHastaNivel(tArbol* arbol,size_t nivel);   // Nodos hasta nivel

size_t cantNodosEnNivel(tArbol* arbol,size_t nivel);      // Nodos en nivel exacto
size_t _cantNodosEnNivel(tArbol* arbol,size_t nivelActual, size_t nivelObjetivo);
size_t cantNodosAPartirDeNivel(tArbol* arbol,size_t nivel);
size_t _cantNodosAPartirDeNivel(tArbol* arbol,size_t nivelActual, size_t nivelObjetivo);

size_t cantNodosSubArbol(tArbol* arbol, void* clave, tCmp cmpClave);  // Nodos del sub�rbol con ra�z dada

size_t cantHijosIzq(tArbol* arbol);              // Nodos con hijo izquierdo
size_t cantHijosSoloIzq(tArbol* arbol);          // Nodos con solo hijo izquierdo

/// ------------------------------------------------------------
/// B�squeda
/// ------------------------------------------------------------
tArbol* busqNodoArbol(tArbol* arbol, void* clave, tCmp cmpClave);            // Buscar nodo
int busqElemArbol(tArbol* arbol, void* destino, size_t tamDestino, tCmp cmpClave);  // Buscar elemento

/// ------------------------------------------------------------
/// M�nimos y M�ximos
/// ------------------------------------------------------------
tArbol* busqNodoMaxArbol(tArbol *arbol);                             // Nodo con valor m�ximo
tArbol* busqNodoMinArbol(tArbol *arbol);                             // Nodo con valor m�nimo
int busqElemMaxArbol(tArbol *arbol, void *info, size_t tamInfo);     // Elemento m�ximo
int busqElemMinArbol(tArbol *arbol, void *info, size_t tamInfo);     // Elemento m�nimo

/// ------------------------------------------------------------
/// Eliminaci�n
/// ------------------------------------------------------------
int eliminarHoja(tArbol* arbol, void* claveHoja, tCmp cmpClave);  // Eliminar hoja espec�fica
int eliminarRaiz(tArbol* arbol);                                 // Eliminar ra�z
int eliminarNodo(tArbol* arbol, void* clave, tCmp cmpClave);      // Eliminar nodo cualquiera
void filterArbol(tArbol *arbol, tFilter filter, void *param);
void eliminarArbol(tArbol* arbol);  // Libera todo el �rbol

/// ------------------------------------------------------------
/// Operaciones sin clave
/// ------------------------------------------------------------
tArbol* busqNodoMinNoClaveArbol(tArbol *arbol, tCmp cmp);
tArbol* _busqNodoMinNoClaveArbol(tArbol *arbol, tArbol *min, tCmp cmp);
int busqElemMinNoClaveArbol(tArbol *arbol, void *info, size_t tamInfo, tCmp cmp);

tArbol* busqNodoMaxNoClaveArbol(tArbol *arbol, tCmp cmp);
tArbol* _busqNodoMaxNoClaveArbol(tArbol *arbol, tArbol *max, tCmp cmp);
int busqElemMaxNoClaveArbol(tArbol *arbol, void *info, size_t tamInfo, tCmp cmp);

tArbol* busqNodoNoClaveArbol(tArbol* arbol, void* info, tCmp cmp);              // Buscar nodo sin clave directa
int busqElemNoClaveArbol(tArbol* arbol, void* destino, size_t tamDestino, tCmp cmp); // Buscar elemento sin clave

void eliminarNodoNoClaveArbol(tArbol *arbol, void *info, tCmp cmp); // Eliminar sin clave directa

#endif // ARBOL_H_INCLUDED
