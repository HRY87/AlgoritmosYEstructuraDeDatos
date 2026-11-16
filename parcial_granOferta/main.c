#include <stdio.h>
#include "vencimientos.h"

#define ARCH_STOCK      "stock.dat"
#define ARCH_VENCER     "prod_a_vencer.txt"
#define TAM_LINEA       50

static int menu(void);
static int ingresarFecha(t_fecha *f);
static unsigned ingresarN(void);
static int informarError(int ret);
static int pedirN(const t_fecha *hoy, unsigned *n);

static void opcionLote(void);
static void opcionFecha(t_fecha *hoy);
static void opcionMostrar(const t_fecha *hoy);
static void opcionGrabar(const t_fecha *hoy);
static void opcionMasProximo(const t_fecha *hoy);

int main(void)
{
    t_fecha hoy = {0, 0, 0}; /* invalida hasta que se ingrese (opcion 2) */
    int op;

    do
    {
        op = menu();

        switch(op)
        {
            case 1: opcionLote();             break;
            case 2: opcionFecha(&hoy);        break;
            case 3: opcionMostrar(&hoy);      break;
            case 4: opcionGrabar(&hoy);       break;
            case 5: opcionMasProximo(&hoy);   break;
            case 0:                           break;
            default: puts("Opcion invalida.");
        }
    }while(op != 0);

    return 0;
}

static int menu(void)
{
    char linea[TAM_LINEA];
    int op = 0;

    puts("\n===== LA GRAN OFERTA =====\n"
         "1 - Generar lote de prueba\n"
         "2 - Ingresar fecha actual\n"
         "3 - Mostrar los n productos mas proximos a vencer\n"
         "4 - Generar archivo prod_a_vencer.txt\n"
         "5 - Mostrar producto mas proximo a vencer\n"
         "0 - Salir");
    printf("Opcion: ");

    if(fgets(linea, sizeof(linea), stdin))
        sscanf(linea, "%d", &op);

    return op;
}

static int ingresarFecha(t_fecha *f)
{
    char linea[TAM_LINEA];
    t_fecha aux;

    printf("Fecha actual (dd/mm/aaaa): ");

    if(fgets(linea, sizeof(linea), stdin)
       && sscanf(linea, "%d/%d/%d", &aux.dia, &aux.mes, &aux.anio) == 3
       && esFechaValida(&aux))
    {
        *f = aux;
        return 1;
    }

    return 0;
}

static unsigned ingresarN(void)
{
    char linea[TAM_LINEA];
    int n = 0;

    printf("Cantidad n de productos: ");

    if(fgets(linea, sizeof(linea), stdin))
        sscanf(linea, "%d", &n);

    return n > 0 ? (unsigned)n : 0;
}

static int informarError(int ret)
{
    if(ret == ERROR_ARCH)
        puts("No se pudo abrir el archivo (genere primero el lote de prueba).");
    else if(ret == SIN_MEM)
        puts("No hay memoria suficiente.");

    return ret;
}

/** Valida que haya fecha actual y pide n **/
static int pedirN(const t_fecha *hoy, unsigned *n)
{
    int ok = esFechaValida(hoy);

    if(!ok)
        puts("Primero ingrese la fecha actual (opcion 2).");
    else
    {
        *n = ingresarN();
        ok = *n > 0;

        if(!ok)
            puts("El valor de n debe ser un entero mayor a 0.");
    }

    return ok;
}

static void opcionLote(void)
{
    if(generarLotePrueba(ARCH_STOCK) == TODO_OK)
        puts("Lote de prueba generado en stock.dat.");
    else
        puts("No se pudo generar el lote de prueba.");
}

static void opcionFecha(t_fecha *hoy)
{
    if(ingresarFecha(hoy))
        puts("Fecha actual registrada.");
    else
        puts("Fecha invalida.");
}

static void opcionMostrar(const t_fecha *hoy)
{
    tLista lista;
    unsigned n;

    crearLista(&lista);

    if(pedirN(hoy, &n)
       && informarError(cargarProximosAVencer(ARCH_STOCK, hoy, n, &lista)) == TODO_OK
       && mostrarProximosAVencer(&lista) == LISTA_VACIA)
    {
        puts("No hay productos con stock vigente.");
    }

    vaciarLista(&lista);
}

static void opcionGrabar(const t_fecha *hoy)
{
    tLista lista;
    unsigned n;

    crearLista(&lista);

    if(pedirN(hoy, &n)
       && informarError(cargarProximosAVencer(ARCH_STOCK, hoy, n, &lista)) == TODO_OK)
    {
        if(grabarProximosAVencer(&lista, ARCH_VENCER) == TODO_OK)
            puts("Archivo prod_a_vencer.txt generado.");
        else
            puts("No se pudo crear prod_a_vencer.txt.");
    }

    vaciarLista(&lista);
}

static void opcionMasProximo(const t_fecha *hoy)
{
    tLista lista;
    tProdVto prod;

    crearLista(&lista);

    if(!esFechaValida(hoy))
        puts("Primero ingrese la fecha actual (opcion 2).");
    else if(informarError(cargarProximosAVencer(ARCH_STOCK, hoy, 1, &lista)) == TODO_OK)
    {
        if(obtenerMasProximoAVencer(&lista, &prod) == TODO_OK)
            printf("\nMas proximo a vencer: %s - %02d/%02d/%04d - cantidad %u\n",
                   prod.codProd, prod.fVto.dia, prod.fVto.mes, prod.fVto.anio, prod.cant);
        else
            puts("No hay productos con stock vigente.");
    }

    vaciarLista(&lista);
}
