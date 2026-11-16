#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaDoble.h"

#define CANT_ELEM(v)    (sizeof(v) / sizeof(*(v)))
#define CANT_MAX_ORD    20

typedef struct
{
    char    codigo[8];
    int     cantidad;
} tProducto;

/**Contadores de resultados**/
static unsigned pruebasTotales = 0;
static unsigned pruebasOk = 0;

/**Genericos: comparacion**/
int cmpInt(const void *a, const void *b);
int cmpProducto(const void *a, const void *b);
int cmpProductoCompleto(const void *a, const void *b);
int cmpTexto(const void *a, const void *b);
int cmpTextoConPuntero(const void *a, const void *b);

/**Genericos: mostrar y acumular**/
void mostrarInt(const void *d);
void mostrarProducto(const void *d);
void mostrarTexto(const void *d);
int acumularProducto(void **info, unsigned *tam, const void *nuevo, unsigned tamNuevo);
int acumularConFalla(void **info, unsigned *tam, const void *nuevo, unsigned tamNuevo);

/**Adaptadores con tamanio fijo de int (para usar con cargarVectorInt)**/
int ponerInicioInt(tListaD *pl, const void *d);
int ponerFinalInt(tListaD *pl, const void *d);
int ponerOrdenInt(tListaD *pl, const void *d);
int eliminarInt(tListaD *pl, int *dato);

/**Auxiliares**/
void titulo(const char *texto);
void comprobar(const char *descripcion, int condicion);
const tNodoD* primerNodo(const tListaD *pl);
int listaIgualA(const tListaD *pl, const void *esperado, size_t tamElem, unsigned cant,
                int (*comparar)(const void*, const void*));
int tamInfoCoincideConTexto(const tListaD *pl);
int cargarVectorInt(tListaD *pl, const int *v, unsigned cant, int (*poner)(tListaD*, const void*));
int imprimirLista(const char *tituloLista, tListaD *pl, void (*mostrar)(const void*),
                  int (*recorrer)(tListaD*, void (*)(const void*)));
int imprimirIzqDer(const char *tituloLista, tListaD *pl, void (*mostrar)(const void*));
void probarOrdenamiento(const char *descripcion, const int *entrada, unsigned cant);

/**Pruebas**/
void pruebaListaVacia(void);
void pruebaPonerAlInicio(void);
void pruebaPonerAlFinal(void);
void pruebaInicioYFinalCombinados(void);
void pruebaPonerEnOrden(void);
void pruebaPonerEnOrdenAcumulando(void);
void pruebaEliminarPorClave(void);
void pruebaOrdenarLista(void);
void pruebaTextos(void);
void pruebaRecorridos(void);
void pruebaVaciar(void);


int main()
{
    pruebaListaVacia();
    pruebaPonerAlInicio();
    pruebaPonerAlFinal();
    pruebaInicioYFinalCombinados();
    pruebaPonerEnOrden();
    pruebaPonerEnOrdenAcumulando();
    pruebaEliminarPorClave();
    pruebaOrdenarLista();
    pruebaTextos();
    pruebaRecorridos();
    pruebaVaciar();

    printf("\n==============================\n");
    printf("RESULTADO: %u de %u pruebas OK\n", pruebasOk, pruebasTotales);

    return pruebasOk == pruebasTotales ? 0 : 1;
}


/** ---------- Pruebas ---------- **/

/* Operaciones sobre una lista recien creada */
void pruebaListaVacia(void)
{
    tListaD lista;
    int dato = 5;

    titulo("Lista vacia");
    crearListaDoble(&lista);

    comprobar("listaDobleVacia es verdadera", listaDobleVacia(&lista));
    comprobar("listaDobleLlena es falsa para un int", !listaDobleLlena(&lista, sizeof(int)));
    comprobar("vaciar una lista vacia devuelve 0", vaciarListaDoble(&lista) == 0);
    comprobar("eliminarPorClave devuelve LISTA_VACIA",
              eliminarPorClave(&lista, &dato, sizeof(int), cmpInt) == LISTA_VACIA);
    comprobar("mostrarDeIzqDer devuelve 0", mostrarDeIzqDerListaDoble(&lista, mostrarInt) == 0);
    comprobar("mostrarDeDerIzq devuelve 0", mostrarDeDerIzqListaDoble(&lista, mostrarInt) == 0);

    ordenarLista(&lista, cmpInt);
    comprobar("ordenarLista no rompe y sigue vacia", listaDobleVacia(&lista));
}

/* Poner al inicio, incluyendo el caso de lista vacia */
void pruebaPonerAlInicio(void)
{
    tListaD lista;
    int v[] = {1, 2, 3};
    int esperado[] = {3, 2, 1};

    titulo("Poner al inicio");
    crearListaDoble(&lista);

    comprobar("en lista vacia devuelve TODO_OK", ponerInicioInt(&lista, v) == TODO_OK);
    comprobar("queda {1}", listaIgualA(&lista, v, sizeof(int), 1, cmpInt));
    comprobar("poner 2 y 3 devuelve TODO_OK", cargarVectorInt(&lista, v + 1, 2, ponerInicioInt) == TODO_OK);
    imprimirIzqDer("Resultado:", &lista, mostrarInt);
    comprobar("queda en orden inverso {3,2,1}", listaIgualA(&lista, esperado, sizeof(int), 3, cmpInt));

    vaciarListaDoble(&lista);
}

/* Poner al final, incluyendo lista vacia y copia independiente del dato */
void pruebaPonerAlFinal(void)
{
    tListaD lista;
    int v[] = {1, 2, 3};
    int dato = 10;
    int copiaEsperada = 10;

    titulo("Poner al final");
    crearListaDoble(&lista);

    comprobar("en lista vacia devuelve TODO_OK", ponerFinalInt(&lista, &dato) == TODO_OK);
    dato = 99;
    comprobar("la lista guarda una copia: sigue en {10}",
              listaIgualA(&lista, &copiaEsperada, sizeof(int), 1, cmpInt));

    vaciarListaDoble(&lista);
    comprobar("cargar {1,2,3} devuelve TODO_OK", cargarVectorInt(&lista, v, 3, ponerFinalInt) == TODO_OK);
    imprimirIzqDer("Resultado:", &lista, mostrarInt);
    comprobar("queda en el mismo orden {1,2,3}", listaIgualA(&lista, v, sizeof(int), 3, cmpInt));

    vaciarListaDoble(&lista);
}

/* Alternar inicio y final: valida los enlaces en ambos extremos */
void pruebaInicioYFinalCombinados(void)
{
    tListaD lista;
    int v[] = {0, 1, 2, 3};
    int ok;

    titulo("Inicio y final combinados");
    crearListaDoble(&lista);

    ok = ponerFinalInt(&lista, v + 2) == TODO_OK;
    ok = ok && ponerInicioInt(&lista, v + 1) == TODO_OK;
    ok = ok && ponerFinalInt(&lista, v + 3) == TODO_OK;
    ok = ok && ponerInicioInt(&lista, v) == TODO_OK;

    comprobar("las 4 inserciones devuelven TODO_OK", ok);
    imprimirIzqDer("Resultado:", &lista, mostrarInt);
    comprobar("queda {0,1,2,3}", listaIgualA(&lista, v, sizeof(int), 4, cmpInt));

    vaciarListaDoble(&lista);
}

/* Insercion ordenada desde distintas posiciones actuales, con duplicados */
void pruebaPonerEnOrden(void)
{
    tListaD lista;
    int v[] = {30, 10, 20, 50, 40};
    int ordenado[] = {10, 20, 30, 40, 50};
    int ordenadoFinal[] = {5, 10, 20, 30, 40, 50, 99};
    int repetido = 30, menor = 5, mayor = 99;

    titulo("Poner en orden");
    crearListaDoble(&lista);

    comprobar("cargar desordenados devuelve TODO_OK", cargarVectorInt(&lista, v, 5, ponerOrdenInt) == TODO_OK);
    imprimirIzqDer("Resultado:", &lista, mostrarInt);
    comprobar("queda {10,20,30,40,50}", listaIgualA(&lista, ordenado, sizeof(int), 5, cmpInt));

    comprobar("un duplicado devuelve DUPLICADO", ponerOrdenInt(&lista, &repetido) == DUPLICADO);
    comprobar("el duplicado no se agrego", listaIgualA(&lista, ordenado, sizeof(int), 5, cmpInt));

    comprobar("poner menor y mayor que todos devuelve TODO_OK",
              ponerOrdenInt(&lista, &menor) == TODO_OK && ponerOrdenInt(&lista, &mayor) == TODO_OK);
    imprimirIzqDer("Con extremos:", &lista, mostrarInt);
    comprobar("queda {5,10,20,30,40,50,99}", listaIgualA(&lista, ordenadoFinal, sizeof(int), 7, cmpInt));

    vaciarListaDoble(&lista);
}

/* Insercion ordenada de estructuras acumulando los duplicados */
void pruebaPonerEnOrdenAcumulando(void)
{
    tListaD lista;
    tProducto ingresos[] = {{"A10", 5}, {"B20", 3}, {"A10", 2}, {"C30", 1}, {"B20", 4}};
    tProducto esperado[] = {{"A10", 7}, {"B20", 7}, {"C30", 1}};
    tProducto repetido = {"A10", 100};
    const tProducto *act = ingresos;
    const tProducto *fin = ingresos + CANT_ELEM(ingresos);
    unsigned duplicados = 0, sinMem = 0;
    int ret;

    titulo("Poner en orden acumulando");
    crearListaDoble(&lista);

    while(act < fin)
    {
        ret = ponerEnOrdenListaDoble(&lista, act, sizeof(tProducto), cmpProducto, acumularProducto);
        duplicados += (ret == DUPLICADO);
        sinMem += (ret == SIN_MEM);
        act++;
    }

    comprobar("hubo 2 duplicados y ninguna falta de memoria", duplicados == 2 && sinMem == 0);
    imprimirIzqDer("Resultado:", &lista, mostrarProducto);
    comprobar("cantidades acumuladas A10:7 B20:7 C30:1",
              listaIgualA(&lista, esperado, sizeof(tProducto), 3, cmpProductoCompleto));

    comprobar("si acumular falla devuelve SIN_MEM",
              ponerEnOrdenListaDoble(&lista, &repetido, sizeof(tProducto), cmpProducto, acumularConFalla) == SIN_MEM);
    comprobar("sin acumular un duplicado devuelve DUPLICADO",
              ponerEnOrdenListaDoble(&lista, &repetido, sizeof(tProducto), cmpProducto, NULL) == DUPLICADO);
    comprobar("las cantidades no cambiaron",
              listaIgualA(&lista, esperado, sizeof(tProducto), 3, cmpProductoCompleto));

    vaciarListaDoble(&lista);
}

/* Eliminar en medio, extremos, claves inexistentes y hasta quedar vacia */
void pruebaEliminarPorClave(void)
{
    tListaD lista;
    int v[] = {10, 20, 30, 40, 50};
    int sinMedio[] = {10, 20, 40, 50};
    int sinPrimero[] = {20, 40, 50};
    int sinUltimo[] = {20, 40};
    int medio = 30, primero = 10, ultimo = 50, inexistente = 99, menor = 5, resto1 = 20, resto2 = 40;
    tProducto productos[] = {{"A10", 1}, {"B20", 2}, {"C30", 3}};
    tProducto buscado = {"B20", 0};
    tProducto sinB20[] = {{"A10", 1}, {"C30", 3}};

    titulo("Eliminar por clave");
    crearListaDoble(&lista);
    cargarVectorInt(&lista, v, 5, ponerOrdenInt);

    comprobar("elimina el del medio (30)", eliminarInt(&lista, &medio) == TODO_OK);
    comprobar("queda {10,20,40,50}", listaIgualA(&lista, sinMedio, sizeof(int), 4, cmpInt));
    comprobar("elimina el primero (10)", eliminarInt(&lista, &primero) == TODO_OK);
    comprobar("queda {20,40,50}", listaIgualA(&lista, sinPrimero, sizeof(int), 3, cmpInt));
    comprobar("elimina el ultimo (50)", eliminarInt(&lista, &ultimo) == TODO_OK);
    comprobar("queda {20,40}", listaIgualA(&lista, sinUltimo, sizeof(int), 2, cmpInt));

    comprobar("clave mayor inexistente devuelve NO_EXISTE", eliminarInt(&lista, &inexistente) == NO_EXISTE);
    comprobar("clave menor inexistente devuelve NO_EXISTE", eliminarInt(&lista, &menor) == NO_EXISTE);
    comprobar("la lista no cambio", listaIgualA(&lista, sinUltimo, sizeof(int), 2, cmpInt));

    comprobar("elimina los dos restantes",
              eliminarInt(&lista, &resto1) == TODO_OK && eliminarInt(&lista, &resto2) == TODO_OK);
    comprobar("la lista queda vacia", listaDobleVacia(&lista));
    comprobar("eliminar en lista vacia devuelve LISTA_VACIA", eliminarInt(&lista, &resto1) == LISTA_VACIA);

    /* Con estructuras: se busca por codigo y se recupera el dato completo */
    ponerEnOrdenListaDoble(&lista, productos, sizeof(tProducto), cmpProducto, NULL);
    ponerEnOrdenListaDoble(&lista, productos + 1, sizeof(tProducto), cmpProducto, NULL);
    ponerEnOrdenListaDoble(&lista, productos + 2, sizeof(tProducto), cmpProducto, NULL);
    comprobar("elimina B20 por codigo",
              eliminarPorClave(&lista, &buscado, sizeof(tProducto), cmpProducto) == TODO_OK);
    comprobar("devuelve el dato completo (cantidad 2)", buscado.cantidad == 2);
    comprobar("quedan A10 y C30",
              listaIgualA(&lista, sinB20, sizeof(tProducto), 2, cmpProductoCompleto));

    vaciarListaDoble(&lista);
}

/* Ordenamiento con distintos casos de entrada */
void pruebaOrdenarLista(void)
{
    int vacia[] = {0};
    int unico[] = {7};
    int dos[] = {2, 1};
    int yaOrdenada[] = {1, 2, 3, 4, 5};
    int inversa[] = {5, 4, 3, 2, 1};
    int mezclada[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    int repetidos[] = {4, 2, 4, 1, 2, 4, 1};
    int iguales[] = {3, 3, 3, 3};
    int grande[] = {15, 3, 12, 7, 20, 1, 18, 9, 14, 5, 11, 2, 19, 8, 16, 4, 13, 6, 17, 10};

    titulo("Ordenar lista");
    probarOrdenamiento("lista vacia", vacia, 0);
    probarOrdenamiento("un solo elemento", unico, 1);
    probarOrdenamiento("dos elementos desordenados", dos, 2);
    probarOrdenamiento("ya ordenada", yaOrdenada, 5);
    probarOrdenamiento("orden inverso", inversa, 5);
    probarOrdenamiento("mezclada (cantidad impar)", mezclada, 9);
    probarOrdenamiento("con repetidos", repetidos, 7);
    probarOrdenamiento("todos iguales", iguales, 4);
    probarOrdenamiento("20 elementos", grande, 20);
}

/* Genericidad con cadenas de distinto tamanio: ordenar y eliminar */
void pruebaTextos(void)
{
    tListaD lista;
    const char *textos[] = {"pera", "manzana", "kiwi", "banana", "uva"};
    const char *esperado[] = {"banana", "kiwi", "manzana", "pera", "uva"};
    const char *sinKiwi[] = {"banana", "manzana", "pera", "uva"};
    const char **act = textos;
    const char **fin = textos + CANT_ELEM(textos);
    char buffer[8] = "kiwi";

    titulo("Textos de distinto tamanio");
    crearListaDoble(&lista);

    while(act < fin)
    {
        ponerAlFinalListaDoble(&lista, *act, strlen(*act) + 1);
        act++;
    }
    imprimirIzqDer("Original:", &lista, mostrarTexto);

    ordenarLista(&lista, cmpTexto);
    imprimirIzqDer("Ordenada:", &lista, mostrarTexto);
    comprobar("queda ordenada alfabeticamente",
              listaIgualA(&lista, esperado, sizeof(char*), 5, cmpTextoConPuntero));
    comprobar("el tamanio de cada nodo sigue coincidiendo con su texto", tamInfoCoincideConTexto(&lista));

    comprobar("elimina \"kiwi\" y recupera el texto",
              eliminarPorClave(&lista, buffer, strlen(buffer) + 1, cmpTexto) == TODO_OK
              && strcmp(buffer, "kiwi") == 0);
    comprobar("queda sin kiwi", listaIgualA(&lista, sinKiwi, sizeof(char*), 4, cmpTextoConPuntero));

    vaciarListaDoble(&lista);
}

/* Los recorridos funcionan desde cualquier nodo actual */
void pruebaRecorridos(void)
{
    tListaD lista;
    int v[] = {4, 1, 3, 2};

    titulo("Recorridos");
    crearListaDoble(&lista);
    cargarVectorInt(&lista, v, 4, ponerOrdenInt); /* el nodo actual queda en el medio */

    comprobar("izquierda a derecha recorre 4 nodos",
              imprimirLista("Izq a Der:", &lista, mostrarInt, mostrarDeIzqDerListaDoble) == 4);
    comprobar("derecha a izquierda recorre 4 nodos",
              imprimirLista("Der a Izq:", &lista, mostrarInt, mostrarDeDerIzqListaDoble) == 4);

    vaciarListaDoble(&lista);
}

/* Vaciar devuelve la cantidad y deja la lista reutilizable */
void pruebaVaciar(void)
{
    tListaD lista;
    int v[] = {5, 1, 4, 2, 3};
    int uno[] = {1};

    titulo("Vaciar");
    crearListaDoble(&lista);
    cargarVectorInt(&lista, v, 5, ponerOrdenInt);

    comprobar("vaciar devuelve la cantidad de nodos (5)", vaciarListaDoble(&lista) == 5);
    comprobar("la lista queda vacia", listaDobleVacia(&lista));
    comprobar("se puede reutilizar luego de vaciar", ponerFinalInt(&lista, v + 1) == TODO_OK
              && listaIgualA(&lista, uno, sizeof(int), 1, cmpInt));
    comprobar("vaciar devuelve 1", vaciarListaDoble(&lista) == 1);
}


/** ---------- Genericos: comparacion ---------- **/

int cmpInt(const void *a, const void *b)
{
    int x = *(const int*)a;
    int y = *(const int*)b;

    return (x > y) - (x < y);
}

int cmpProducto(const void *a, const void *b)
{
    return strcmp(((const tProducto*)a)->codigo, ((const tProducto*)b)->codigo);
}

/* Devuelve 0 solo si coinciden codigo y cantidad */
int cmpProductoCompleto(const void *a, const void *b)
{
    return cmpProducto(a, b) != 0 || ((const tProducto*)a)->cantidad != ((const tProducto*)b)->cantidad;
}

int cmpTexto(const void *a, const void *b)
{
    return strcmp((const char*)a, (const char*)b);
}

/* Compara un texto contra un elemento de un vector de const char* */
int cmpTextoConPuntero(const void *a, const void *b)
{
    return strcmp((const char*)a, *(const char* const*)b);
}


/** ---------- Genericos: mostrar y acumular ---------- **/

/* Las funciones mostrar reciben NULL una vez al inicio (encabezado): aqui no imprimen nada */
void mostrarInt(const void *d)
{
    if(d)
        printf("%d ", *(const int*)d);
}

void mostrarProducto(const void *d)
{
    if(d)
        printf("[%s x%d] ", ((const tProducto*)d)->codigo, ((const tProducto*)d)->cantidad);
}

void mostrarTexto(const void *d)
{
    if(d)
        printf("%s ", (const char*)d);
}

/* Suma la cantidad del producto nuevo al ya guardado */
int acumularProducto(void **info, unsigned *tam, const void *nuevo, unsigned tamNuevo)
{
    (void)tam;
    (void)tamNuevo;
    ((tProducto*)*info)->cantidad += ((const tProducto*)nuevo)->cantidad;
    return 1;
}

/* Simula una falla al acumular (devuelve 0 como si no hubiera memoria) */
int acumularConFalla(void **info, unsigned *tam, const void *nuevo, unsigned tamNuevo)
{
    (void)info;
    (void)tam;
    (void)nuevo;
    (void)tamNuevo;
    return 0;
}


/** ---------- Adaptadores con tamanio de int ---------- **/

int ponerInicioInt(tListaD *pl, const void *d)
{
    return ponerAlInicioListaDoble(pl, d, sizeof(int));
}

int ponerFinalInt(tListaD *pl, const void *d)
{
    return ponerAlFinalListaDoble(pl, d, sizeof(int));
}

int ponerOrdenInt(tListaD *pl, const void *d)
{
    return ponerEnOrdenListaDoble(pl, d, sizeof(int), cmpInt, NULL);
}

int eliminarInt(tListaD *pl, int *dato)
{
    return eliminarPorClave(pl, dato, sizeof(int), cmpInt);
}


/** ---------- Auxiliares ---------- **/

void titulo(const char *texto)
{
    printf("\n=== %s ===\n", texto);
}

/* Registra y muestra el resultado de una comprobacion */
void comprobar(const char *descripcion, int condicion)
{
    pruebasTotales++;
    pruebasOk += (condicion != 0);
    printf("  [%s] %s\n", condicion ? "OK" : "FALLA", descripcion);
}

/* Devuelve el nodo mas a la izquierda (o NULL si la lista esta vacia) */
const tNodoD* primerNodo(const tListaD *pl)
{
    const tNodoD *act = *pl;

    while(act && act->ant)
        act = act->ant;

    return act;
}

/* Compara la lista con un vector esperado y valida que cada ant apunte al nodo previo */
int listaIgualA(const tListaD *pl, const void *esperado, size_t tamElem, unsigned cant,
                int (*comparar)(const void*, const void*))
{
    const tNodoD *act = primerNodo(pl);
    const tNodoD *previo = NULL;
    const char *elem = (const char*)esperado;
    unsigned i = 0;
    int coincide = 1;

    while(act && i < cant && coincide)
    {
        coincide = act->ant == previo && comparar(act->info, elem) == 0;
        previo = act;
        act = act->sig;
        elem += tamElem;
        i++;
    }

    return coincide && i == cant && !act;
}

/* Verifica que tamInfo de cada nodo sea el largo del texto + 1 */
int tamInfoCoincideConTexto(const tListaD *pl)
{
    const tNodoD *act = primerNodo(pl);
    int coincide = 1;

    while(act && coincide)
    {
        coincide = act->tamInfo == strlen((const char*)act->info) + 1;
        act = act->sig;
    }

    return coincide;
}

/* Carga un vector de enteros usando la primitiva indicada; SIN_MEM si alguna fallo */
int cargarVectorInt(tListaD *pl, const int *v, unsigned cant, int (*poner)(tListaD*, const void*))
{
    const int *fin = v + cant;
    int ret = TODO_OK;

    while(v < fin && ret != SIN_MEM)
    {
        ret = poner(pl, v);
        v++;
    }

    return ret == SIN_MEM ? SIN_MEM : TODO_OK;
}

/* Muestra la lista con el recorrido indicado y devuelve la cantidad de nodos mostrados */
int imprimirLista(const char *tituloLista, tListaD *pl, void (*mostrar)(const void*),
                  int (*recorrer)(tListaD*, void (*)(const void*)))
{
    int cant;

    printf("  %-14s", tituloLista);
    cant = recorrer(pl, mostrar);
    printf("%s\n", cant ? "" : "(vacia)");

    return cant;
}

int imprimirIzqDer(const char *tituloLista, tListaD *pl, void (*mostrar)(const void*))
{
    return imprimirLista(tituloLista, pl, mostrar, mostrarDeIzqDerListaDoble);
}

/* Carga la entrada, la ordena y compara contra una copia ordenada con qsort */
void probarOrdenamiento(const char *descripcion, const int *entrada, unsigned cant)
{
    tListaD lista;
    int ordenado[CANT_MAX_ORD];

    memcpy(ordenado, entrada, cant * sizeof(int));
    qsort(ordenado, cant, sizeof(int), cmpInt);

    crearListaDoble(&lista);
    cargarVectorInt(&lista, entrada, cant, ponerFinalInt);
    ordenarLista(&lista, cmpInt);

    comprobar(descripcion, listaIgualA(&lista, ordenado, sizeof(int), cant, cmpInt));

    vaciarListaDoble(&lista);
}
