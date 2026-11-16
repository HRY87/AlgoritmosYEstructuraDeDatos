#include <stdio.h>
#include "lote.h"

#define PREFIJO_APROBADOS   "lotesAprobados"
#define PREFIJO_OBSERVADOS  "lotesObservados"
#define TMP_APROBADOS_FALLA "aprobadosFalla.tmp"
#define TMP_OBSERVADOS_OK   "observadosOk.tmp"
#define TAM_NOMBRE          40
#define TAM_LINEA           100

/* Del lote en curso solo se guarda lo que cambia por registro:
   el codLote es el mismo para todos y se conserva una sola vez. */
typedef struct
{
    int     idProducto;
    char    esOk;
}tItem;

typedef struct
{
    FILE    *aprobados,      /* final: primero OK            */
            *observados,     /* final: primero FALLA         */
            *tmpAprFalla,    /* FALLA de aprobados (se anexa) */
            *tmpObsOk;       /* OK de observados (se anexa)   */
}tSalidas;

typedef struct
{
    FILE        *pf;
    const char  *codLote;
    char        esOk;
}tEscritura;

/**Auxiliares (privadas)**/
static int cmpItem(const void *a, const void *b);
static void escribirSiCoincide(void *info, void *param);
static void volcarGrupo(tLista *pl, FILE *pf, const char *codLote, char esOk);
static int esAprobado(unsigned cantOk, unsigned cant, int x);
static void cerrarLote(tLista *pl, const char *codLote, int aprobado, tSalidas *sal);
static int clasificarLotes(FILE *pfEnt, int x, tSalidas *sal);
static void armarNombreSalida(char *dest, size_t tam, const char *prefijo, int x);
static int abrirSalidas(tSalidas *sal, int x);
static void cerrarSalidas(tSalidas *sal);
static void cerrarArchivo(FILE *pf);
static void anexarArchivo(FILE *destino, FILE *origen);
static int mostrarArchivoTexto(const char *path, const char *encabezado);


/**Proceso principal: el archivo de entrada se lee una sola vez**/
int procesarEntrada(const char *pathEntrada, int x)
{
    FILE *pfEnt = fopen(pathEntrada, "rb");
    tSalidas sal;
    int ret;

    if(!pfEnt)
        return ERROR_ARCH;

    if(!abrirSalidas(&sal, x))
    {
        fclose(pfEnt);
        return ERROR_ARCH;
    }

    ret = clasificarLotes(pfEnt, x, &sal);

    if(ret == TODO_OK)
    {
        anexarArchivo(sal.observados, sal.tmpObsOk);
        anexarArchivo(sal.aprobados, sal.tmpAprFalla);
    }

    cerrarSalidas(&sal);
    fclose(pfEnt);

    return ret;
}

static int clasificarLotes(FILE *pfEnt, int x, tSalidas *sal)
{
    tRegistro   reg;
    tItem       item;
    tLista      lote;
    char        codAct[TAM_COD_LOTE];
    unsigned    cant, cantOk;
    int         ret = TODO_OK,
                leido;

    crearLista(&lote);
    leido = fread(&reg, sizeof(reg), 1, pfEnt) == 1;

    while(leido && ret == TODO_OK)
    {
        strcpy(codAct, reg.codLote);
        cant = cantOk = 0;

        while(leido && ret == TODO_OK && strcmp(codAct, reg.codLote) == 0)
        {
            item.idProducto = reg.idProducto;
            item.esOk = strcmp(reg.resultadoControl, RES_OK) == 0;

            ret = ponerEnOrdenLista(&lote, &item, sizeof(item), cmpItem);

            cant++;
            cantOk += item.esOk;
            leido = fread(&reg, sizeof(reg), 1, pfEnt) == 1;
        }

        if(ret == TODO_OK)
            cerrarLote(&lote, codAct, esAprobado(cantOk, cant, x), sal);

        vaciarLista(&lote); /* libera el lote en curso, siempre */
    }

    return ret;
}

/* cantOk/cant >= x/100  <=>  cantOk*100 >= x*cant (sin decimales) */
static int esAprobado(unsigned cantOk, unsigned cant, int x)
{
    return (unsigned long)cantOk * 100 >= (unsigned long)x * cant;
}

static void cerrarLote(tLista *pl, const char *codLote, int aprobado, tSalidas *sal)
{
    if(aprobado)
    {
        volcarGrupo(pl, sal->aprobados, codLote, 1);
        volcarGrupo(pl, sal->tmpAprFalla, codLote, 0);
    }
    else
    {
        volcarGrupo(pl, sal->observados, codLote, 0);
        volcarGrupo(pl, sal->tmpObsOk, codLote, 1);
    }
}

static void volcarGrupo(tLista *pl, FILE *pf, const char *codLote, char esOk)
{
    tEscritura esc;

    esc.pf = pf;
    esc.codLote = codLote;
    esc.esOk = esOk;

    recorrerLista(pl, escribirSiCoincide, &esc);
}

static void escribirSiCoincide(void *info, void *param)
{
    const tItem *item = (const tItem*)info;
    const tEscritura *esc = (const tEscritura*)param;

    if(item->esOk == esc->esOk)
        fprintf(esc->pf, "%s,%d,%s\n", esc->codLote, item->idProducto,
                item->esOk ? RES_OK : RES_FALLA);
}

static int cmpItem(const void *a, const void *b)
{
    int x = ((const tItem*)a)->idProducto;
    int y = ((const tItem*)b)->idProducto;

    return (x > y) - (x < y);
}


/**Archivos de salida**/
static void armarNombreSalida(char *dest, size_t tam, const char *prefijo, int x)
{
    snprintf(dest, tam, "%s-%d.txt", prefijo, x);
}

static int abrirSalidas(tSalidas *sal, int x)
{
    char nomApr[TAM_NOMBRE], nomObs[TAM_NOMBRE];

    armarNombreSalida(nomApr, sizeof(nomApr), PREFIJO_APROBADOS, x);
    armarNombreSalida(nomObs, sizeof(nomObs), PREFIJO_OBSERVADOS, x);

    sal->aprobados = fopen(nomApr, "wt");
    sal->observados = fopen(nomObs, "wt");
    sal->tmpAprFalla = fopen(TMP_APROBADOS_FALLA, "w+t");
    sal->tmpObsOk = fopen(TMP_OBSERVADOS_OK, "w+t");

    if(!sal->aprobados || !sal->observados || !sal->tmpAprFalla || !sal->tmpObsOk)
    {
        cerrarSalidas(sal);
        return 0;
    }

    return 1;
}

static void cerrarArchivo(FILE *pf)
{
    if(pf)
        fclose(pf);
}

static void cerrarSalidas(tSalidas *sal)
{
    cerrarArchivo(sal->aprobados);
    cerrarArchivo(sal->observados);
    cerrarArchivo(sal->tmpAprFalla);
    cerrarArchivo(sal->tmpObsOk);

    remove(TMP_APROBADOS_FALLA);
    remove(TMP_OBSERVADOS_OK);
}

static void anexarArchivo(FILE *destino, FILE *origen)
{
    int c;

    rewind(origen);

    while((c = fgetc(origen)) != EOF)
        fputc(c, destino);
}


/**Mostrar**/
int mostrarEntrada(const char *pathEntrada)
{
    FILE *pf = fopen(pathEntrada, "rb");
    tRegistro reg;
    unsigned cant = 0;

    if(!pf)
        return ERROR_ARCH;

    printf("\n=== Archivo de entrada (%s) ===\n", pathEntrada);
    printf("%-10s %-12s %-10s\n", "codLote", "idProducto", "resultado");

    while(fread(&reg, sizeof(reg), 1, pf) == 1)
    {
        printf("%-10s %-12d %-10s\n", reg.codLote, reg.idProducto, reg.resultadoControl);
        cant++;
    }

    printf("Total de registros: %u\n", cant);
    fclose(pf);

    return TODO_OK;
}

int mostrarSalidas(int x)
{
    char nomApr[TAM_NOMBRE], nomObs[TAM_NOMBRE];
    int r1, r2;

    armarNombreSalida(nomApr, sizeof(nomApr), PREFIJO_APROBADOS, x);
    armarNombreSalida(nomObs, sizeof(nomObs), PREFIJO_OBSERVADOS, x);

    r1 = mostrarArchivoTexto(nomApr, "LOTES APROBADOS (primero OK, luego FALLA)");
    r2 = mostrarArchivoTexto(nomObs, "LOTES OBSERVADOS (primero FALLA, luego OK)");

    return (r1 == TODO_OK && r2 == TODO_OK) ? TODO_OK : ERROR_ARCH;
}

static int mostrarArchivoTexto(const char *path, const char *encabezado)
{
    FILE *pf = fopen(path, "rt");
    char linea[TAM_LINEA];

    if(!pf)
        return ERROR_ARCH;

    printf("\n=== %s - %s ===\n", encabezado, path);
    printf("codLote,idProducto,resultadoControl\n");

    while(fgets(linea, sizeof(linea), pf))
        fputs(linea, stdout);

    fclose(pf);

    return TODO_OK;
}


/**Lote de prueba**/
int generarLoteDePrueba(const char *path)
{
    static const tRegistro prueba[] = {
        /* L001: 4 de 5 OK = 80% justo en el limite, ids desordenados */
        {"L001", 105, "OK"}, {"L001", 101, "OK"}, {"L001", 103, "OK"},
        {"L001", 102, "OK"}, {"L001", 104, "FALLA"},
        /* L002: 1 de 4 OK = 25% */
        {"L002", 203, "FALLA"}, {"L002", 201, "FALLA"},
        {"L002", 204, "OK"},    {"L002", 202, "FALLA"},
        /* L003: un solo registro OK = 100% */
        {"L003", 301, "OK"},
        /* L004: todo FALLA = 0% */
        {"L004", 402, "FALLA"}, {"L004", 401, "FALLA"}, {"L004", 403, "FALLA"},
        /* L005: 7 de 10 OK = 70% */
        {"L005", 510, "OK"},    {"L005", 501, "FALLA"}, {"L005", 508, "OK"},
        {"L005", 503, "OK"},    {"L005", 505, "FALLA"}, {"L005", 502, "OK"},
        {"L005", 509, "OK"},    {"L005", 504, "OK"},    {"L005", 507, "FALLA"},
        {"L005", 506, "OK"},
        /* L006: 2 de 3 OK = 66,67% (distingue entre x=66 y x=67) */
        {"L006", 603, "OK"}, {"L006", 601, "FALLA"}, {"L006", 602, "OK"}
    };
    FILE *pf = fopen(path, "wb");

    if(!pf)
        return ERROR_ARCH;

    fwrite(prueba, sizeof(tRegistro), sizeof(prueba) / sizeof(*prueba), pf);
    fclose(pf);

    return TODO_OK;
}
