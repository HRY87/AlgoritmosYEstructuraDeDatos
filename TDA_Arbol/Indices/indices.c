#include "indices.h"

/// ==========================================================================
/// Funciones Auxiliares
/// ==========================================================================

int cmpIndice(const void *a, const void *b)
{
    tInd *indA = (tInd*)a, *indB = (tInd*)b;
    return indA->clave - indB->clave;
}

/// ==========================================================================
/// Funciones Principales
/// ==========================================================================

// Crea un archivo índice desde el archivo original (recorre y guarda claves)
int crearArchInd(char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave)
{
    // Creamos variables
    FILE *archInd, *archOri;
    void *buffer;
    tArbol arbol;
    tInd ind;
    
    // Preparamos variables
    ind.pos = 0;

    crearArbol(&arbol);

    if(!(archInd = fopen(nombArchInd,"wb")))
        return ERR_ARCH;

    if(!(archOri = fopen(nombArchOri,"rb")))
    {
        fclose(archInd);
        return ERR_ARCH;
    }

    if(!(buffer = malloc(tamInfo)))
    {
        fclose(archInd);
        fclose(archOri);
        return SIN_MEM;
    }

    // Recorremos el archivo original
    while(fread(buffer, tamInfo, 1, archOri) == 1)
    {
        // Insertamos la clave del registro al tInd
        extraerClave(buffer, &ind.clave);

        // Insertamos en el arbol
        if (insArb(&arbol, &ind, sizeof(ind), cmpIndice) != EXITO)
        {
            free(buffer);
            fclose(archInd);
            fclose(archOri);
            return ERR_INSER;
        }

        // Aumentamos contador
        ind.pos++;
    }

    // Escribimos el contenido del arbol al archivo indice
    crearDesdeArchBinArbol(archInd,&arbol);

    // Liberamos memoria y archivos
    free(buffer);
    fclose(archInd);
    fclose(archOri);
    return EXITO;
}

// Funciones para la estructura de Contexo
int inicializarIndCtx(tIndCtx *ctx, char *nombArchOri, char *nombArchInd, size_t tamInfo, tCmp cmpClave, tExtraer extraerClave)
{
    if(!cmpClave || !extraerClave)
        return ERR_NO_INICIALIZADO;

    // Abrimos archivos y guardamos en el contexto
    if (!(ctx->archOri = fopen(nombArchOri, "r+b")))
        return ERR_ARCH;

    if (!(ctx->archInd = fopen(nombArchInd, "r+b")))
    {
        fclose(ctx->archOri);
        return ERR_ARCH;
    }

    // Escribimos los argumentos al contextos
    ctx->tamInfo = tamInfo;
    ctx->cmpClave = cmpClave;
    ctx->extraerClave = extraerClave;
    strcpy(ctx->nombArchOri, nombArchOri);
    strcpy(ctx->nombArchInd, nombArchInd);
    crearArbol(&ctx->arbol);

    // Cargamos el contenido del archivo indice al arbol
    cargarDesdeArchBinOrdArbol(ctx->archInd, &ctx->arbol, sizeof(tInd));
    fclose(ctx->archInd);

    return EXITO;
}

int finalizarIndCtx(tIndCtx *ctx)
{
    // Creamos archivo indice desde cero
    if(!(ctx->archInd = fopen(ctx->nombArchInd, "wb")))
        return ERR_ARCH;

    // Guardamos el contenido del arbol al archivo
    crearDesdeArchBinArbol(ctx->archInd, &ctx->arbol);

    // Cerramos archivos y liberamos arbol
    fclose(ctx->archOri);
    fclose(ctx->archInd);
    eliminarArbol(&ctx->arbol);

    return EXITO;
}

// Inserta un nuevo registro en el archivo original y actualiza el índice
int insArchInd(tIndCtx *ctx, void *info)
{
    tInd ind;

    // Ir al final del archivo original
    fseek(ctx->archOri, 0L, SEEK_END);

    // Crear indice
    ind.pos = ftell(ctx->archOri) / ctx->tamInfo;
    ctx->extraerClave(info, &ind.clave);

    // Escribir el nuevo registro en el archivo original
    if (fwrite(info, ctx->tamInfo, 1, ctx->archOri) != 1)
        return ERR_ARCH;

    // Insertar en el árbol de índice
    return insArb(&ctx->arbol, &ind, sizeof(tInd), cmpIndice);
}

// Busca un registro según la clave, lo carga en info
int busqArchInd(tIndCtx *ctx, void *info)
{
    tInd ind;
    ctx->extraerClave(info,&ind.clave);

    // Buscamos contenido indice y copiamos a 'ind'
    if(busqElemArbol(&ctx->arbol,&ind,sizeof(ind),cmpIndice) != EXITO)
        return NO_ENCONTRADO;

    // Leemos la posición obtenida al archivo original
    return leerPosDesdeArchBin(ctx->archOri,info,ctx->tamInfo,ind.pos);
}

// Modifica un registro existente con la clave dada
int modArchBinind(tIndCtx *ctx, void *info, tAccion accion, void *param)
{
    tInd ind;
    ctx->extraerClave(info,&ind.clave);

    // Buscamos contenido indice y copiamos a 'ind'
    if(busqElemArbol(&ctx->arbol,&ind,sizeof(ind),cmpIndice) != EXITO)
        return NO_ENCONTRADO;

    // Leemos la posición obtenida al archivo original
    leerPosDesdeArchBin(ctx->archOri,info,ctx->tamInfo,ind.pos);

    // Aplicar la acción modificadora
    accion(info, param);

    // Reescribir el registro modificado en la misma posición
    return escribirPosDesdeArchBin(ctx->archOri,info,ctx->tamInfo,ind.pos);
}

// Eliminación logica de un registro al eliminar el nodo
int elimArchInd(tIndCtx *ctx, void *info)
{
    tInd ind;
    ctx->extraerClave(info, &ind.clave);
    return eliminarNodo(&ctx->arbol,&ind,cmpIndice);
}

// Recorre el archivo original en orden (según el árbol índice) y aplica la acción
void _recorrerArchInd(void *info, size_t tamInfo, size_t nivel, void *param)
{
    tIndCtxReco *contx = (tIndCtxReco *)param;
    tInd *indice = (tInd*)info;
    void *buffer;

    if (!(buffer = malloc(contx->ctx->tamInfo)))
        return;

    if (leerPosDesdeArchBin(contx->ctx->archOri, buffer, contx->ctx->tamInfo, indice->pos) == EXITO)
        contx->accion(buffer, contx->param);

    free(buffer);
}

int recorrerArchInd(tIndCtx *ctx, tAccion accion, void *param)
{
    tIndCtxReco contx = { ctx, accion, param };
    recorrerIn(&ctx->arbol, _recorrerArchInd, &contx);
    return EXITO;
}

// Filtra el árbol índice eliminando nodos cuyos registros del archivo original no cumplan el filtro.
int _filtrarArchInd(void *info, void *param)
{
    void *buffer;
    tInd *ind = (tInd*)info;
    tIndCtxFiltro *ctx = (tIndCtxFiltro*)param;

    if (!(buffer = malloc(ctx->ctx->tamInfo)))
        return SIN_MEM;

    if (leerPosDesdeArchBin(ctx->ctx->archOri, buffer, ctx->ctx->tamInfo, ind->pos) != EXITO)
    {
        free(buffer);
        return 0;
    }

    int res = ctx->filtroUsuario(buffer, ctx->param);
    free(buffer);

    return res;
}
int filtrarArchInd(tIndCtx *ctx, tFilter filtroUsuario, void *param)
{
    tIndCtxFiltro contexto = { ctx, filtroUsuario, param };
    filterArbol(&ctx->arbol, _filtrarArchInd, &contexto);
    return EXITO;
}

/// ==========================================================================
/// Utilidades
/// ==========================================================================

/// Muestra encabezado de �ndice con clave entera
void mostrarIndCabe()
{
    printf("%5s %5s\n", "CLAVE", "POS");
}

/// Muestra una entrada de �ndice con clave entera
void mostrarInd(void *info, void *param)
{
    const tInd *ind = (const tInd *)info;
    printf("%5d %5zu\n", ind->clave, ind->pos);
}
