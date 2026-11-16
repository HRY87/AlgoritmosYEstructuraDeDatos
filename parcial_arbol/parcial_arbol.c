#include "parcial_arbol.h"

// Comparacion sensible a mayusculas (ver sugerencias)
int cmpPalabra(const void *a, const void *b)
{
    return strcmp((const char*)a, (const char*)b);
}

// Libera todo el arbol en postorden (hijos primero, despues el nodo)
void vaciarArbolBinBusq(tArbolBinBusq *p)
{
    if(*p)
    {
        vaciarArbolBinBusq(&(*p)->izq);
        vaciarArbolBinBusq(&(*p)->der);
        free((*p)->info);
        free(*p);
        *p = NULL;
    }
}

/// ==========================================================================
/// Carga del arbol desde archivo de texto
/// ==========================================================================

// Cierra la palabra acumulada (si hay) y la inserta si no estaba.
// Como insertar devuelve 0 tanto para duplicado como para falta de memoria,
// primero se consulta existencia (tam = 0: no copia nada, solo busca).
int _volcarPalabra(tArbolBinBusq *arbol, tAcumPal *acum)
{
    int res = TODO_BIEN;

    if(acum->fin != acum->pal)      // fin == pal => no hay nada acumulado
    {
        *acum->fin = '\0';

        if(!buscarElemArbolBinBusq(arbol, acum->pal, 0, cmpPalabra))
            // Tamanio justo: largo + '\0'. Si devuelve 0 aca, es SIN_MEM
            res = insertarArbolBinBusq(arbol, acum->pal,
                                       (unsigned)(acum->fin - acum->pal) + 1, cmpPalabra);

        acum->fin = acum->pal;      // Acumulador listo para la proxima palabra
    }

    return res;
}

// Recorre 'lon' caracteres con aritmetica de punteros:
// alfanumerico => se acumula; cualquier otro => delimitador, cierra la palabra
int _procesarLinea(const char *linea, size_t lon, tArbolBinBusq *arbol, tAcumPal *acum)
{
    const char *fin = linea + lon;
    int res = TODO_BIEN;

    while(linea < fin && res == TODO_BIEN)
    {
        if(isalnum((unsigned char)*linea))
        {
            if(acum->fin < acum->pal + MAX_PALABRA)     // Descarta lo que excede el tope
                *acum->fin++ = *linea;
        }
        else
            res = _volcarPalabra(arbol, acum);

        linea++;
    }

    return res;
}

int cargarDesdeArchTxtPalabrasArbol(FILE *arch, tArbolBinBusq *arbol)
{
    char linea[MAXLINEA + 2];       // MAXLINEA chars + '\n' + '\0'
    tAcumPal acum;
    size_t lon;
    int res = TODO_BIEN;

    if(!arch)
        return ERROR_ARCH;

    acum.fin = acum.pal;

    while(res == TODO_BIEN && fgets(linea, sizeof(linea), arch))
    {
        lon = strcspn(linea, "\r\n");       // Largo real sin salto de linea

        res = _procesarLinea(linea, lon, arbol, &acum);

        // Linea corta => termino natural => palabra completa.
        // Linea llena => puede haberse cortado: queda pendiente para la siguiente.
        if(res == TODO_BIEN && lon < MAXLINEA)
            res = _volcarPalabra(arbol, &acum);
    }

    // Si el archivo termina con una linea llena, queda una palabra pendiente
    if(res == TODO_BIEN)
        res = _volcarPalabra(arbol, &acum);

    return res;
}

int cargarArchTxtPalabrasArbol(const char *nombArch, tArbolBinBusq *arbol)
{
    FILE *arch;
    int res;

    if(!(arch = fopen(nombArch, "rt")))
        return ERROR_ARCH;

    res = cargarDesdeArchTxtPalabrasArbol(arch, arbol);

    fclose(arch);
    return res;
}

/// ==========================================================================
/// Diferencia A - B
/// ==========================================================================

// Accion de recorrerEnOrden: si la palabra de A no esta en B, la muestra y la guarda.
// tam = 0 => solo consulta existencia, no modifica el nodo de A.
void _difArbol(void *info, unsigned tam, unsigned nivel, void *param)
{
    tCtxDif *ctx = (tCtxDif*)param;

    if(!buscarElemArbolBinBusq(ctx->arbolB, info, 0, cmpPalabra))
    {
        printf("%s\n", (char*)info);
        fprintf(ctx->arch, "%s\n", (char*)info);
        ctx->cant++;
    }
}

int difDesdeArchTxtArbol(tArbolBinBusq *a, tArbolBinBusq *b, FILE *arch)
{
    tCtxDif ctx = { b, arch, 0 };

    if(!arch)
        return ERROR_ARCH;

    puts("Palabras de A que no estan en B:");

    recorrerEnOrdenArbolBinBusq(a, &ctx, _difArbol);   // Inorden => salen ordenadas

    printf("Total: %u palabra(s)\n", ctx.cant);
    return TODO_BIEN;
}

int difArchTxtArbol(tArbolBinBusq *a, tArbolBinBusq *b, const char *nombArchSal)
{
    FILE *arch;
    int res;

    if(!(arch = fopen(nombArchSal, "wt")))
        return ERROR_ARCH;

    res = difDesdeArchTxtArbol(a, b, arch);

    fclose(arch);
    return res;
}
