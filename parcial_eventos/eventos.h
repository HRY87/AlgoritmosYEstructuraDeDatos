#ifndef EVENTOS_H_INCLUDED
#define EVENTOS_H_INCLUDED

#include "colaCircular.h" /** Comparte TODO_OK y SIN_MEM con el TDA **/

#define ARCH_EVENTOS    "eventos.txt"
#define ARCH_ALERTAS    "alertas.txt"

#define TAM_LINEA       100

#define NIVEL_MIN       1
#define NIVEL_MAX       5
#define NIVEL_CRITICO   NIVEL_MAX
#define CANT_CRITICA    3  //Alarmas consecutivas de nivel critico

#define ERROR_ARCH      -2
#define EVENTO_INVALIDO  2
#define ALERTA_GENERADA  3

typedef struct
{
    int codDisp;
    int fecha;  //AAAAMMDD
    int hora;   //HHMMSS
    int alarma; //1..5
}tEvento;

int parsearEvento(const char* linea, tEvento* ev);

/** Devuelve la cantidad de alertas generadas, ERROR_ARCH o SIN_MEM **/
int procesarEventos(const char* pathEnt, const char* pathSal);

/** Recibe NULL una vez al inicio (encabezado) y despues cada linea **/
int mostrarArchivo(const char* path, const char* titulo, void (*mostrar)(const char*));
void mostrarEvento(const char* linea);
void mostrarAlerta(const char* linea);

#endif // EVENTOS_H_INCLUDED
