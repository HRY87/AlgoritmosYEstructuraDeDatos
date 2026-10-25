#include <stdio.h>
#include <string.h>
#include "pilaDinamica.h"

/**Producto**/
typedef struct
{
    char    codProd[11],
            descrip[46];
} tProd;

int ingresarProducto(tProd *d);
void mostrarProducto(const tProd *d);

/**Linea de texto**/
int ingresarTexto(char *linea, int tamLinea);

void probarIngresarYMostrarProd(void);
void probarIngresarYMostrarTexto(void);
void probarPonerYSacarDePila(void);


int main(void)
{
    probarIngresarYMostrarProd();

    probarIngresarYMostrarTexto();

    probarPonerYSacarDePila();

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


int _probarLlenaYApilar(tPila *pila)
{
    tProd   prod;
    int     cant = 0;

    puts("Probando pila llena y poner en pila.");
    mostrarProducto(NULL);
    while(!pilaLlena(pila, sizeof(prod)) && ingresarProducto(&prod))
    {
        if(ponerEnPila(pila, &prod, sizeof(prod)) != TODO_OK)
        {
            fprintf(stderr, "ERROR - inesperado - pila llena.\n");
            puts("no se pudo cargar la informacion y"
                 " habria que tomar alguna decision drastica.");
        }
        mostrarProducto(&prod);
        cant++;
    }
    return cant;
}


void _probarVerTope(tPila *pila)
{
    tProd   prod;

    puts("Probando ver el tope de la pila.");
    if(verTopePila(pila, &prod, sizeof(prod)) == TODO_OK)
    {
        mostrarProducto(NULL);
        mostrarProducto(&prod);
    }
    else
        puts("La pila estaba vacia.");
    puts("");
}


void _probarVaciaYDesapilarN(tPila *pila, int canti)
{
    tProd   prod;

    printf("Probando pila vacia y sacar de pila %d productos (mostrandolos.\n",
           canti);
    if(pilaVacia(pila))
        puts("La pila esta vacia.");
    else
        mostrarProducto(NULL);
    while(canti > 0 && sacarDePila(pila, &prod, sizeof(prod)) == TODO_OK)
    {
        canti--;
        mostrarProducto(&prod);
    }
    puts("");
}


int _probarVaciarPilaYPilaVacia(tPila *pila)
{
    puts("Probando vaciar pila y pila vacia.");
    vaciarPila(pila);
    if(!pilaVacia(pila))
        return 0; // fprintf(stderr, "ERROR - la pila debia estar vacia\n\n");
    printf("Vaciar pila funciona!\n\n");
    puts("");
    return 1;
}


void _probarLlenaYApilarTexto(tPila *pila)
{
    char    linea[70];
    int     cant = 0;

    puts("Probando pila llena y poner en pila Texto.");
    while(!pilaLlena(pila, sizeof(linea)) &&
          ingresarTexto(linea, sizeof(linea)))
    {
        if(ponerEnPila(pila, linea, strlen(linea) + 1) != TODO_OK)
        {
            fprintf(stderr, "ERROR - inesperado - pila llena.\n");
            puts("no se pudo cargar la informacion y"
                 " habria que tomar alguna decision drastica.");
        }
        printf("\"%s\"\n", linea);
        cant++;
    }
    printf("se pusieron %d lineas de texto en la pila.\n\n", cant);
    printf("Probando sacar de pila con las lineas de texto.\n");
}


void _probarSacarDePilaTexto(tPila *pila)
{
    char    linea[70];
    int     cant = 0;

    while(sacarDePila(pila, linea, sizeof(linea)) == TODO_OK)
    {
        cant++;
        printf("\"%s\"\n", linea);
    }
    printf("Se sacaron y mostraron %d lineas de texto\n\n", cant);
}


void probarPonerYSacarDePila(void)
{
    tPila   pila;
    int     cant;

    crearPila(&pila);

    puts("Probando primitivas de pila con productos.\n"
         "======== ========== == ==== === =========\n");
    cant = _probarLlenaYApilar(&pila);
    printf("se pusieron %d productos en la pila.\n\n", cant);

    _probarVerTope(&pila);

    _probarVaciaYDesapilarN(&pila, cant - 2);

    _probarVerTope(&pila);

    if(_probarVaciarPilaYPilaVacia(&pila) != 1)
        fprintf(stderr, "ERROR - inesperado, la pila NO QUEDO vacia\n");

    puts("Probando primitivas de pila con lineas de texto.\n"
         "======== ========== == ==== === ====== == =====\n");

    _probarLlenaYApilarTexto(&pila);

    _probarSacarDePilaTexto(&pila);
    cant = 0;

    puts("ATENCION: se mostro el uso de una pila, en la que se apilaron"
         " productos, se pro\n"
         "baron todas las primitivas. Una vez que se la dejo vacia "
         "se apilaron lineas\n"
         "de texto y luego se desapilaron. Lo mas importante de esto "
         "no es utilizar la\n"
         "misma pila, lo que mas importa es que con las mismas primiti"
         "vas se pueden api-\n"
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

/**Linea de texto**/
int ingresarTexto(char *linea, int tamLinea)
{
    static const char *textos[] = {
        "Hola mundo",
        "Las pilas guardan datos de distinto tamano",
        "Ultimo en entrar, primero en salir",
        "Linea corta",
        "Esta linea de texto es un poco mas larga que las anteriores",
        "Programacion en C con punteros y memoria dinamica",
        "El tope de la pila es lo ultimo que se apilo",
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
