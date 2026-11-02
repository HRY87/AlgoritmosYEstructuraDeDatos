#include "TDA_Arbol_Arch.h"

/// ==========================================================================
/// Cargar Archivo Desordenado a un Arbol
/// ==========================================================================

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

int cargarDesdeArchBinDesArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tCmp cmpClave)
{
    void *buffer;

    if(!arch)
        return ERR_ARCH;

    if(!(buffer = malloc(tamInfo)))
        return SIN_MEM;

    rewind(arch);

    while(fread(buffer, tamInfo, 1, arch) == 1)
        insArb(arbol, buffer, tamInfo, cmpClave);

    free(buffer);
    return EXITO;
}
int cargarArchBinDesArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tCmp cmpClave)
{
    FILE *arch;
    int res;

    if(!(arch = fopen(nombArch,"rb")))
        return ERR_ARCH;

    res = cargarDesdeArchBinDesArbol(arch,arbol,tamInfo,cmpClave);

    fclose(arch);
    return res;
}

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

int cargarDesdeArchTxtDesArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tCmp cmpClave, tTrozador leerTrozado)
{
    void *buffer;
    char linea[BUFFER];

    if(!arch)
        return ERR_ARCH;

    if(!(buffer = malloc(tamInfo)))
        return SIN_MEM;

    while(fgets(linea, BUFFER, arch) && leerTrozado(linea, buffer) == EXITO)
    {
        insArb(arbol, buffer, tamInfo, cmpClave);
    }

    free(buffer);
    return EXITO;
}
int cargarArchTxtDesArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tCmp cmpClave, tTrozador leerTrozado)
{
    FILE *arch;
    int res;

    if(!(arch = fopen(nombArch,"rt")))
        return ERR_ARCH;

    res = cargarDesdeArchTxtDesArbol(arch,arbol,tamInfo,cmpClave,leerTrozado);

    fclose(arch);
    return res;
}

/// ==========================================================================
/// Funciones de arbol con archivo Ordenado
/// ==========================================================================

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

/*
* - Usa una estrategia de divisi�n por mitades (tipo �rbol completo balanceado).
* - Elige el elemento del medio (m) como ra�z.
* - Repite recursivamente con los tramos izquierdo y derecho para formar sub�rboles.
*/
int _cargarDesdeArchBinOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo, int li, int ls)
{
    if (li > ls)
        return EXITO;

    int m = (li + ls) / 2;
    void *buffer;

    if (!(buffer = malloc(tamInfo)))
        return SIN_MEM;

    // Leer el elemento en la posicion m
    fseek(arch, m * tamInfo, SEEK_SET);
    if (fread(buffer, tamInfo, 1, arch) != 1)
    {
        free(buffer);
        return ERR_ARCH;
    }

    // Reservar nodo e insertar el dato leído
    *arbol = malloc(sizeof(tNodo));
    if (!*arbol)
    {
        free(buffer);
        return SIN_MEM;
    }

    (*arbol)->info = buffer;
    (*arbol)->tamInfo = tamInfo;
    (*arbol)->izq = NULL;
    (*arbol)->der = NULL;

    // Recursión para construir subárboles izquierdo y derecho
    int resIzq = _cargarDesdeArchBinOrdArbol(arch, &(*arbol)->izq, tamInfo, li, m - 1);
    int resDer = _cargarDesdeArchBinOrdArbol(arch, &(*arbol)->der, tamInfo, m + 1, ls);
    return resIzq != EXITO ? resIzq : resDer;
}
int cargarDesdeArchBinOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo)
{
    fseek(arch, 0L, SEEK_END);
    return _cargarDesdeArchBinOrdArbol(arch, arbol, tamInfo, 0, ftell(arch) / tamInfo - 1);
}
int cargarArchBinOrdArbol(char *nombArch, tArbol *arbol, size_t tamInfo)
{
    FILE *arch;
    int  res;

    if (*arbol) // Si el árbol ya está cargado, no se sobrescribe
        return 0;

    if (!(arch = fopen(nombArch, "rb")))
        return ERR_ARCH;

    res = cargarDesdeArchBinOrdArbol(arch, arbol, tamInfo);

    fclose(arch);
    return res;
}

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

int _cargarDesdeArchTxtOrdArbol(FILE *arch, tArbol *arbol, size_t tamInfo, tTrozador leerTrozado, int li, int ls)
{
    if (li > ls)
        return EXITO;

    int m = (li + ls) / 2;
    char linea[BUFFER];
    void *buffer;

    if (!(buffer = malloc(tamInfo)))
        return SIN_MEM;

    // Ir a la línea m desde el inicio del archivo
    rewind(arch);
    for (int i = 0; i <= m; i++) {
        if (!fgets(linea, BUFFER, arch)) {
            free(buffer);
            return ERR_ARCH;
        }
    }

    // Convertir línea a estructura
    if (leerTrozado(linea, buffer) != EXITO)
    {
        free(buffer);
        return ERR_ARCH;
    }

    // Crear el nodo y asignar la información
    if (!(*arbol = malloc(sizeof(tNodo))))
    {
        free(buffer);
        return SIN_MEM;
    }

    (*arbol)->info = buffer;
    (*arbol)->tamInfo = tamInfo;
    (*arbol)->izq = NULL;
    (*arbol)->der = NULL;

    // Construir recursivamente subárbol izquierdo y derecho
    int resIzq = _cargarDesdeArchTxtOrdArbol(arch, &(*arbol)->izq, tamInfo, leerTrozado, li, m - 1);
    int resDer = _cargarDesdeArchTxtOrdArbol(arch, &(*arbol)->der, tamInfo, leerTrozado, m + 1, ls);

    return resIzq != EXITO ? resIzq : resDer;
}
int cargarDesdeArchTxtOrdArbol(FILE *arch, tArbol *arbol , size_t tamInfo, tTrozador leerTrozado)
{
    rewind(arch);
    int cantReg = 0;
    char aux[BUFFER];

    while (fgets(aux, BUFFER, arch))
        cantReg++;

    return _cargarDesdeArchTxtOrdArbol(arch, arbol, tamInfo, leerTrozado, 0, cantReg - 1);
}
int cargarArchTxtOrdArbol(char *nombArch, tArbol *arbol, size_t tamInfo, tTrozador leerTrozado)
{
    FILE *arch;
    int  res;

    if (*arbol)
        return 0;

    if (!(arch = fopen(nombArch, "rt")))
        return ERR_ARCH;

    res = cargarDesdeArchTxtOrdArbol(arch, arbol, tamInfo, leerTrozado);

    fclose(arch);
    return res;
}

/// ==========================================================================
/// Creación de Archivos a Partir del Contenido de un Arbol
/// ==========================================================================+

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

void _crearDesdeArchBinArbol(void *info, size_t tamInfo, size_t nivel, void *param)
{
    fwrite(info, tamInfo, 1, param);
}
int crearDesdeArchBinArbol(FILE *arch, tArbol *arbol)
{
    rewind(arch);
    recorrerIn(arbol, _crearDesdeArchBinArbol, arch);
}
int crearArchBinArbol(char *nombArch, tArbol *arbol)
{
    FILE *arch;

    if(!(arch = fopen(nombArch, "wb")))
    {
        return ERR_ARCH;
    }

    crearDesdeArchBinArbol(arch,arbol);

    fclose(arch);
    return EXITO;
}

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

void _crearArchTxtArbol(void *info, size_t tamInfo, size_t nivel, void *param)
{
    tContextoArchTxt *contexto = (tContextoArchTxt*)param;

    char linea[256];
    contexto->crearTrozado(linea, info);  // Usamos la función de trozado
    fputs(linea, contexto->arch);         // Escribimos la línea en el archivo
}
int crearDesdeArchTxtArbol(FILE *arch, tArbol *arbol, tTrozador crearTrozado)
{
    tContextoArchTxt contexto = { arch, crearTrozado };
    recorrerIn(arbol, _crearArchTxtArbol, &contexto);
}
int crearArchTxtArbol(char *nombArch, tArbol *arbol, tTrozador crearTrozado)
{
    FILE *arch;

    if(!(arch = fopen(nombArch,"wt")))
        return ERR_ARCH;

    crearDesdeArchTxtArbol(arch, arbol, crearTrozado);

    fclose(arch);
    return EXITO;
}
