#include <stdio.h>
#include <ctype.h>
#include "lote.h"

#define SIN_PORCENTAJE  -1

char menu(void);
int ingresarPorcentaje(void);
void informar(int ret, const char *exito);

int main()
{
    int x = SIN_PORCENTAJE;
    char op;

    do
    {
        op = menu();

        switch(op)
        {
        case 'a':
            x = ingresarPorcentaje();
            break;
        case 'b':
            if(x == SIN_PORCENTAJE)
                puts("Primero ingrese el porcentaje (opcion a).");
            else
                informar(procesarEntrada(ARCH_ENTRADA, x), "Archivos generados.");
            break;
        case 'c':
            informar(mostrarEntrada(ARCH_ENTRADA), "");
            break;
        case 'd':
            if(x == SIN_PORCENTAJE)
                puts("Primero ingrese el porcentaje (opcion a).");
            else
                informar(mostrarSalidas(x), "");
            break;
        case 'e':
            informar(generarLoteDePrueba(ARCH_ENTRADA), "Lote de prueba generado.");
            break;
        case 's':
            break;
        default:
            puts("Opcion invalida.");
        }
    }
    while(op != 's');

    return 0;
}

char menu(void)
{
    char linea[16];
    char op = 0;

    printf("\n===== CONTROL DE CALIDAD POR LOTE =====\n"
           "a) Ingresar porcentaje x\n"
           "b) Procesar archivo de entrada\n"
           "c) Mostrar archivo de entrada\n"
           "d) Mostrar archivos de salida\n"
           "e) Generar lote de prueba\n"
           "s) Salir\n"
           "Opcion: ");

    if(fgets(linea, sizeof(linea), stdin))
        sscanf(linea, " %c", &op);

    return tolower((unsigned char)op);
}

int ingresarPorcentaje(void)
{
    char linea[16];
    int x = SIN_PORCENTAJE;

    printf("Porcentaje x (entero 0-100): ");

    if(!fgets(linea, sizeof(linea), stdin) || sscanf(linea, "%d", &x) != 1
       || x < 0 || x > 100)
    {
        puts("Valor invalido.");
        return SIN_PORCENTAJE;
    }

    printf("x = %d%%\n", x);
    return x;
}

void informar(int ret, const char *exito)
{
    if(ret == TODO_OK)
        puts(exito);
    else if(ret == SIN_MEM)
        puts("ERROR: sin memoria.");
    else
        puts("ERROR: no se pudo abrir el archivo (¿ya genero/proceso el lote?).");
}
