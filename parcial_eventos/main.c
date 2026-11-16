#include <stdio.h>
#include <ctype.h>
#include "eventos.h"

#define OP_PROCESAR     'A'
#define OP_MOSTRAR_ENT  'B'
#define OP_MOSTRAR_SAL  'C'
#define OP_SALIR        'S'

char menu(void);
void ejecutarOpcion(char op);
void opcionProcesar(void);
void opcionMostrar(const char* path, const char* titulo, void (*mostrar)(const char*));

int main()
{
    char op;

    do
    {
        op = menu();
        ejecutarOpcion(op);
    }
    while(op != OP_SALIR);

    return 0;
}

char menu(void)
{
    char linea[TAM_LINEA];

    printf("\n===== MONITOREO INDUSTRIAL =====\n"
           "A. Procesar archivo de entrada\n"
           "B. Mostrar archivo de entrada\n"
           "C. Mostrar archivo de salida\n"
           "S. Salir\n"
           "Opcion: ");

    if(!fgets(linea, sizeof(linea), stdin))
        return OP_SALIR;

    return (char)toupper((unsigned char)*linea);
}

void ejecutarOpcion(char op)
{
    switch(op)
    {
        case OP_PROCESAR:
            opcionProcesar();
            break;
        case OP_MOSTRAR_ENT:
            opcionMostrar(ARCH_EVENTOS, "Archivo de entrada", mostrarEvento);
            break;
        case OP_MOSTRAR_SAL:
            opcionMostrar(ARCH_ALERTAS, "Dispositivos en estado critico", mostrarAlerta);
            break;
        case OP_SALIR:
            break;
        default:
            puts("Opcion invalida.");
    }
}

void opcionProcesar(void)
{
    int ret = procesarEventos(ARCH_EVENTOS, ARCH_ALERTAS);

    if(ret == ERROR_ARCH)
        printf("No se pudo abrir '%s' o crear '%s'.\n", ARCH_EVENTOS, ARCH_ALERTAS);
    else if(ret == SIN_MEM)
        puts("Sin memoria: el procesamiento se interrumpio.");
    else
        printf("Procesado OK: %d dispositivo(s) en estado critico -> %s\n", ret, ARCH_ALERTAS);
}

void opcionMostrar(const char* path, const char* titulo, void (*mostrar)(const char*))
{
    int cant = mostrarArchivo(path, titulo, mostrar);

    if(cant == ERROR_ARCH)
        printf("No se pudo abrir '%s'. Si es la salida, primero use la opcion A.\n", path);
    else
        printf("Registros: %d\n", cant);
}
