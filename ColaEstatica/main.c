#include <stdio.h>
#include <string.h>

#include "colaEstatica.h"


/**Productos**/

typedef struct
{
    char    codProd[11],
            descrip[46];
} tProd;

int ingresarProducto(tProd *d);
void mostrarProducto(const tProd *d);


/**LineasTexto**/
int ingresarTexto(char *linea, int tamLinea);


void probarIngresarYMostrarProd(void);
void probarIngresarYMostrarTexto(void);
void probarPonerYSacarDeCola(void);


int main(void)
{
    probarIngresarYMostrarProd();

    probarIngresarYMostrarTexto();

    probarPonerYSacarDeCola();

    return 0;
}


void probarIngresarYMostrarProd(void)
{
    tProd   prod;
    int     cant = 0;

    puts("Probando ingresar productos y mostrar productos.\n"
         "======== ======== ========= = ======= =========");
    if(ingresarProducto(&prod))
        mostrarProducto(NULL);
    do
    {
        mostrarProducto(&prod);
        cant++;
    } while(ingresarProducto(&prod));
    fprintf(stdout, "Se mostraron %d productos.\n\n", cant);
}

void probarIngresarYMostrarTexto(void)
{
    char    linea[90];
    int     cant = 0;

    puts("Probando ingresar lineas de texto mostrandolas.\n"
         "======== ======== ====== == ===== ============");
    while(ingresarTexto(linea, sizeof(linea)))
    {
        cant++;
        printf("\"%s\"\n", linea);
    }
    fprintf(stdout, "Se mostraron %d lineas de texto.\n\n", cant);
}

int _probarLlenaYEncolar(tCola *cola)
{
    tProd   prod;
    int     cant = 0;

    puts("Probando cola llena y poner en cola.");
    mostrarProducto(NULL);
    while(!colaLlena(cola, sizeof(prod)) && ingresarProducto(&prod))
    {
        if(ponerEnCola(cola, &prod, sizeof(prod)) != TODO_OK)
        {
            fprintf(stderr, "ERROR - inesperado - cola llena.\n");
            puts("no se pudo cargar la informacion y"
                 " habria que tomar alguna decision drastica.");
        }
        mostrarProducto(&prod);
        cant++;
    }
    return cant;
}


void _probarVerTope(tCola *cola)
{
    tProd   prod;

    puts("Probando ver el primero de la cola.");
    if(verPrimeroCola(cola, &prod, sizeof(prod)) == TODO_OK)
    {
        mostrarProducto(NULL);
        mostrarProducto(&prod);
    }
    else
        puts("La cola estaba vacia.");
    puts("");
}


void _probarVaciaYDesacolarN(tCola *cola, int canti)
{
    tProd   prod;

    printf("Probando cola vacia y sacar de cola %d productos (mostrandolos.\n",
        canti);
    if(colaVacia(cola))
        puts("La cola esta vacia.");
    else
        mostrarProducto(NULL);
    while(canti > 0 && sacarDeCola(cola, &prod, sizeof(prod)) == TODO_OK)
    {
        canti--;
        mostrarProducto(&prod);
    }
    puts("");
}


int _probarVaciarColaYColaVacia(tCola *cola)
{
    puts("Probando vaciar cola y cola vacia.");
    vaciarCola(cola);
    if(!colaVacia(cola))
        return 0; // fprintf(stderr, "ERROR - la cola debia estar vacia\n\n");
    printf("Vaciar cola funciona!\n\n");
    puts("");
    return 1;
}


void _probarLlenaYEncolarTexto(tCola *cola)
{
    char    linea[70];
    int     cant = 0;

    puts("Probando cola llena y poner en cola Texto.");
    while(!colaLlena(cola, sizeof(linea)) &&
        ingresarTexto(linea, sizeof(linea)))
    {
        if(ponerEnCola(cola, linea, strlen(linea) + 1) != TODO_OK)
        {
            fprintf(stderr, "ERROR - inesperado - cola llena.\n");
            puts("no se pudo cargar la informacion y"
                " habria que tomar alguna decision drastica.");
        }
        printf("\"%s\"\n", linea);
        cant++;
    }
    printf("se pusieron %d lineas de texto en la cola.\n\n", cant);
    printf("Probando sacar de cola con las lineas de texto.\n");
}


void _probarSacarDeColaTexto(tCola *cola)
{
    char    linea[70];
    int     cant = 0;

    while(sacarDeCola(cola, linea, sizeof(linea)) == TODO_OK)
    {
        cant++;
        printf("\"%s\"\n", linea);
    }
    printf("Se sacaron y mostraron %d lineas de texto\n\n", cant);
}


void probarPonerYSacarDeCola(void)
{
    tCola   cola;
    int     cant;

    crearCola(&cola);

    puts("Probando primitivas de cola con productos.\n"
        "======== ========== == ==== === =========\n");
    cant = _probarLlenaYEncolar(&cola);
    printf("se pusieron %d productos en la cola.\n\n", cant);

    _probarVerTope(&cola);

    _probarVaciaYDesacolarN(&cola, cant - 2);

    _probarVerTope(&cola);

    if(_probarVaciarColaYColaVacia(&cola) != 1)
        fprintf(stderr, "ERROR - inesperado, la cola NO QUEDO vacia\n");

    puts("Probando primitivas de cola con lineas de texto.\n"
        "======== ========== == ==== === ====== == =====\n");

    _probarLlenaYEncolarTexto(&cola);

    _probarSacarDeColaTexto(&cola);
    cant = 0;

    puts("ATENCION: se mostro el uso de una cola, en la que se pusieron"
        " productos, se pro\n"
        "baron todas las primitivas. Una vez que se la dejo vacia "
        "se encolaron lineas\n"
        "de texto y luego se desencolaron. Lo mas importante de esto "
        "no es utilizar la\n"
        "misma cola, lo que mas importa es que con las mismas primiti"
        "vas se pueden enco-\n"
        "lar distintos tipos de objetos, incluyendo lineas de texto d"
        "e distinto tamano.\n");
}


/**Producto**/
int ingresarProducto(tProd *d)
{
    static const tProd productos[] = {
        ///1234567890    123456789 123456789 123456789 123456789 12345
        { "clavoro3/4", "Clavo de oro 24 kilates de 3/4 de pulgada" },
        { "martillo3K", "Martillo bolita con saca clavos de 3 kilos"},
        { "alamyeso1", "Alambre de yeso de un milimetro de espesor" },
        { "rem-vid15", "Remache de vidrio de 1,5 milimetros" },
        { "plom-telgo", "Plomada de poliestireno expandido" },
        { "limagoma17", "Lima de goma de 17 pulgadas"} };
    static const tProd *act = productos;
    const tProd *fin = productos + sizeof(productos) / sizeof(*productos);

    if(act == fin)
    {
        act = productos;
        return 0;
    }
    *d = *act;
    act++;

    return 1;
}


void mostrarProducto(const tProd *d)
{
    if(d)
        fprintf(stdout,
                "%-*s %-*s ...\n",
                (int)sizeof(d->codProd) - 1, d->codProd,
                (int)sizeof(d->descrip) - 1, d->descrip);
    else
        fprintf(stdout,
                "%-*.*s %-*.*s ...\n",
                (int)sizeof(d->codProd) - 1, (int)sizeof(d->codProd) - 1,
                    "Cod. Producto",
                (int)sizeof(d->descrip) - 1, (int)sizeof(d->descrip) - 1,
                    "Descripcion del producto");
}


/**LineasTexto**/
int ingresarTexto(char *linea, int tamLinea)
{
    static const char *textos[] = {
        "Hola mundo",
        "Las colas guardan datos de distinto tamano",
        "Primero en entrar, primero en salir",
        "Linea corta",
        "Esta linea de texto es un poco mas larga que las anteriores",
        "Programacion en C con punteros y memoria dinamica",
        "El primero de la cola es el que mas espera",
        "Fin" };
    static const char **act = textos;
    const char **fin = textos + sizeof(textos) / sizeof(*textos);

    if(act == fin)
    {
        act = textos;
        return 0;
    }
    snprintf(linea, tamLinea, "%s", *act);
    act++;

    return 1;
}
