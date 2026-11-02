#include "indicesInt.h"


/// ----------------------------------------
/// Busqueda en IndiceInt
/// ----------------------------------------

int busqBinDesdeArchIndiceInt(FILE *archInd, int clave, size_t *pos)
{
    if (!archInd)
    {
        return ERR_ARCH;
    }

    fseek(archInd, 0L, SEEK_END);
    size_t total = ftell(archInd) / sizeof(tIndiceInt);
    size_t li = 0, ls = total - 1, medio;
    tIndiceInt ind;
    int res;

    if (total == 0)
    {
        return NO_ENCONTRADO;
    }

    while (li <= ls)
    {
        medio = (li + ls) / 2;

        fseek(archInd, medio * sizeof(tIndiceInt), SEEK_SET);

        if (fread(&ind, sizeof(tIndiceInt), 1, archInd) != 1)
            return ERR_ARCH;

        res = ind.clave - clave;

        if (!res)
        {
            *pos = ind.pos;
            return EXITO;
        }
        else if (res > 0)
        {
            ls = medio - 1;
        }
        else
        {
            li = medio + 1;
        }
    }

    return NO_ENCONTRADO;
}

int busqBinArchIndiceInt(char *nombArchInd, int clave, size_t *pos)
{
    FILE *arch = fopen(nombArchInd, "rb");
    if (!arch)
    {
        return ERR_ARCH;
    }

    int res = busqBinDesdeArchIndiceInt(arch, clave, pos);

    fclose(arch);
    return res;
}

/// ----------------------------------------
/// Archivos Binarios
/// ----------------------------------------

int crearDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave, tFilter filter, void *param)
{
    tArbol arbol;
    tIndiceInt IndiceInt;
    void *buffer;
    size_t pos = 0;

    if (!archOri || !archInd)
    {
        return ERR_ARCH;
    }

    if (!(buffer = malloc(tamInfo)))
    {
        return SIN_MEM;
    }

    crearArbol(&arbol);

    while (fread(buffer, tamInfo, 1, archOri) == 1)
    {
        // Filtrar antes de agregar al �ndice
        if (!filter || filter(buffer, param) == EXITO)
        {
            IndiceInt.pos = pos;
            extraerClave(buffer, &IndiceInt.clave);

            if (rinsArb(&arbol, &IndiceInt, sizeof(tIndiceInt), cmpClave) != EXITO)
            {
                free(buffer);
                eliminarArbol(&arbol);
                return ERR_INSER;
            }
        }
        pos++;
    }

    recorrerIn(&arbol, _crearArchBinArbol, archInd);

    free(buffer);
    eliminarArbol(&arbol);
    return EXITO;
}

int crearArchBinIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave, tFilter filter, void *param)
{
    FILE *archOri, *archInd;
    int res;

    if (!(archOri = fopen(nombArchOri, "rb")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "wb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    res = crearDesdeArchBinIndiceInt(archOri, archInd, tamInfo, cmpClave, extraerClave, filter, param);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int recorrerDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo, tAccion accion, void *param)
{
    void *buffer;
    tIndiceInt IndiceInt;

    if(!archOri || !archInd)
    {
        return ERR_ARCH;
    }

    if(!(buffer = malloc(tamInfo)))
    {
        return SIN_MEM;
    }

    rewind(archOri);
    rewind(archInd);

    while(fread(&IndiceInt, sizeof(IndiceInt), 1, archInd) == 1)
    {
        if(leerPosDesdeArchBin(archOri,buffer,tamInfo,IndiceInt.pos) == EXITO)
            accion(buffer, param);
    }

    free(buffer);
    return EXITO;
}
int recorrerArchBinIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo, tAccion accion, void *param)
{
    FILE *archOri, *archInd;
    int res;

    if (!(archOri = fopen(nombArchOri, "rb")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "rb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    res = recorrerDesdeArchBinIndiceInt(archOri, archInd, tamInfo, accion, param);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int busqDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, int clave, void *destino, size_t tamDes)
{
    if (!archOri || !archInd || !destino)
    {
        return ERR_ARCH;
    }

    size_t pos;
    int res;

    if ((res = busqBinDesdeArchIndiceInt(archInd, clave, &pos)) != EXITO)
    {
        return res;
    }

    return leerPosDesdeArchBin(archOri, destino, tamDes, pos);
}
int busqArchBinIndiceInt(char *nombArchOri, char *nombArchInd, int clave, void *destino, size_t tamDes)
{
    FILE *archOri, *archInd;

    if (!(archOri = fopen(nombArchOri, "rb")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "rb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    int res = busqDesdeArchBinIndiceInt(archOri, archInd, clave, destino, tamDes);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int modDesdeArchBinIndiceInt(FILE *archOri, FILE *archInd, int clave, void *info, size_t tamInfo)
{
    if (!archOri || !archInd || !info)
    {
        return ERR_ARCH;
    }

    size_t pos;
    int res;

    if ((res = busqBinDesdeArchIndiceInt(archInd, clave, &pos)) != EXITO)
    {
        return res;
    }

    fseek(archOri, pos * tamInfo, SEEK_SET);

    if (fwrite(info, tamInfo, 1, archOri) != 1)
    {
        return ERR_ARCH;
    }

    return EXITO;
}
int modArchBinIndiceInt(char *nombArchOri, char *nombArchInd, int clave, void *info, size_t tamInfo)
{
    FILE *archOri, *archInd;

    if (!(archOri = fopen(nombArchOri, "rb+")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "rb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    int res = modDesdeArchBinIndiceInt(archOri, archInd, clave, info, tamInfo);

    fclose(archOri);
    fclose(archInd);
    return res;
}

/// ----------------------------------------
/// Archivos Texto
/// ----------------------------------------

int crearDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave, tTrozador leerTrozado, tFilter filter, void *param)
{
    tArbol arbol;
    tIndiceInt IndiceInt;
    void *buffer;
    char linea[BUFFER];
    size_t pos = 0;

    if (!archOri || !archInd)
    {
        return ERR_ARCH;
    }

    if (!(buffer = malloc(tamInfo)))
    {
        return SIN_MEM;
    }

    crearArbol(&arbol);

    while (fgets(linea, BUFFER, archOri) && leerTrozado(linea, buffer) == EXITO)
    {
        if (!filter || filter(buffer, param) == 1)
        {
            IndiceInt.pos = pos;
            extraerClave(buffer, &IndiceInt.clave);

            if (rinsArb(&arbol, &IndiceInt, sizeof(tIndiceInt), cmpClave) != EXITO)
            {
                free(buffer);
                eliminarArbol(&arbol);
                return ERR_INSER;
            }
        }

        pos++;
    }

    recorrerIn(&arbol, _crearArchBinArbol, archInd);

    free(buffer);
    eliminarArbol(&arbol);
    return EXITO;
}
int crearArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave, tTrozador leerTrozado, tFilter filter, void *param)
{
    FILE *archOri, *archInd;
    int res;

    if (!(archOri = fopen(nombArchOri, "rt")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "wb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    res = crearDesdeArchTxtIndiceInt(archOri, archInd, tamInfo, cmpClave, extraerClave, leerTrozado, filter, param);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int recorrerDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd, size_t tamInfo, tAccion accion, void *param, tTrozador leerTrozado)
{
    void *buffer;
    char linea[BUFFER];
    tIndiceInt IndiceInt;

    if(!archOri || !archInd)
    {
        return ERR_ARCH;
    }

    if(!(buffer = malloc(tamInfo)))
    {
        return SIN_MEM;
    }

    rewind(archOri);
    rewind(archInd);

    while(fread(&IndiceInt, sizeof(IndiceInt), 1, archInd) == 1)
    {
        if(leerPosDesdeArchBin(archOri,buffer,tamInfo,IndiceInt.pos) == EXITO)
        {
            leerTrozado(linea, buffer);
            accion(buffer, param);
        }
    }

    free(buffer);
    return EXITO;
}
int recorrerArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, size_t tamInfo, tAccion accion, void *param, tTrozador leerTrozado)
{
    FILE *archOri, *archInd;
    int res;

    if (!(archOri = fopen(nombArchOri, "rb")))
    {
        return ERR_ARCH;
    }

    if (!(archInd = fopen(nombArchInd, "rb")))
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    res = recorrerDesdeArchTxtIndiceInt(archOri, archInd, tamInfo, accion, param, leerTrozado);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int busqDesdeArchTxtIndiceInt(FILE *archOri, FILE *archInd, int clave, void *destino, tTrozador leerTrozado)
{
    if (!archOri || !archInd || !destino || !leerTrozado)
    {
        return ERR_ARCH;
    }

    size_t pos;
    int res;
    char linea[BUFFER];
    size_t actual = 0;

    if ((res = busqBinDesdeArchIndiceInt(archInd, clave, &pos)) != EXITO)
    {
        return res;
    }

    rewind(archOri);

    while (fgets(linea, sizeof(linea), archOri))
    {
        if (actual == pos)
        {
             return leerTrozado(linea, destino);
        }

        actual++;
    }

    return NO_ENCONTRADO;
}

int busqArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, int clave, void *destino, tTrozador leerTrozado)
{
    FILE *archOri = fopen(nombArchOri, "rt");
    if (!archOri)
        return ERR_ARCH;

    FILE *archInd = fopen(nombArchInd, "rb");
    if (!archInd)
    {
        fclose(archOri);
        return ERR_ARCH;
    }

    int res = busqDesdeArchTxtIndiceInt(archOri, archInd, clave, destino, leerTrozado);

    fclose(archOri);
    fclose(archInd);
    return res;
}

int modArchTxtIndiceInt(char *nombArchOri, char *nombArchInd, int clave, void *nuevo, size_t tamInfo, tTrozador leerTrozado, int crearTrozado(char*, void*))
{
    FILE *archOri = fopen(nombArchOri, "rt");
    FILE *archInd = fopen(nombArchInd, "rb");
    FILE *archTmp = fopen("aux.txt", "wt");

    if (!archOri || !archInd || !archTmp)
    {
        if (archOri) fclose(archOri);
        if (archInd) fclose(archInd);
        if (archTmp) fclose(archTmp);
        return ERR_ARCH;
    }

    size_t pos, actual = 0;
    char linea[BUFFER];
    int res = busqBinDesdeArchIndiceInt(archInd, clave, &pos);

    if (res != EXITO)
    {
        fclose(archOri);
        fclose(archInd);
        fclose(archTmp);
        return res;
    }

    while (fgets(linea, sizeof(linea), archOri))
    {
        if (actual == pos)
        {
            char nuevaLinea[BUFFER];
            crearTrozado(nuevaLinea, nuevo);
            fputs(nuevaLinea, archTmp);
        }
        else
        {
            fputs(linea, archTmp);
        }
        actual++;
    }

    fclose(archOri);
    fclose(archInd);
    fclose(archTmp);

    remove(nombArchOri);
    rename("aux.txt", nombArchOri);
    return EXITO;
}
