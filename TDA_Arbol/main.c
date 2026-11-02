#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Indices/indices.h"
#include "utilidades.h"

#define LOTE_ALUM "alumnos.bin"
#define LOTE_ALUM_ORD "alumnos_ord.bin"
#define IND_ALUM "alumnos.idx"

void mostrarInt(void *info, void *param);

int main()
{
    testArchivosGen();
    testArbol();
    remove("alumnos.bin.idx");
    testArchivosArbol();
    testInd();
}

int testArchivosGen()
{
    puts("\n==========================================================");
    puts("TESTEANDO LIBRERIA DE ARCHIVOS GENERICOS");
    puts("==========================================================");
    puts("");

    int pos;
    tAlu alu;

    printf(">>> Generando lote de alumnos...\n");
    if(generarLoteAlu(LOTE_ALUM) != EXITO)
    {
        printf("- Error al crear el archivo.\n");
        return ERR_ARCH;
    }
    else
        printf("- Archivo creado correctamente.\n");

    printf("\n>>> Leyendo todos los registros:\n");
    mostrarAluCabe();
    leerArchBin(LOTE_ALUM, sizeof(tAlu), mostrarAlu, NULL);
    printf("\n");

    pos = 2;
    printf("\n>>> Leyendo posicion %d:\n" ,pos);
    if (leerPosArchBin(LOTE_ALUM, &alu, sizeof(tAlu), pos) == EXITO)
    {
        printf("- Valor leido:\n");
        mostrarAluCabe();
        mostrarAlu(&alu,NULL);
    }
    else
    {
        printf("- Error al leer posición %d.\n", pos);
        return ERR_ARCH;
    }

    strcpy(alu.nombre,"Jime");
    alu.legajo = 100;
    *alu.carrera = '\0';
    alu.estado = 'B';
    alu.promedio = 10;
    pos = 3;
    printf("\n>>> Escribiendo alumno en la posicion %d:\n", pos);
    if (escribirPosArchBin(LOTE_ALUM, &alu, sizeof(tAlu), pos) == EXITO)
        printf("- Escritura exitosa.\n");
    else
    {
        printf("- Error al escribir en posición %d.\n",pos);
        return ERR_INSER;
    }

    printf("\n>>> Leyendo todos los registros:\n");
    mostrarAluCabe();
    leerArchBin(LOTE_ALUM, sizeof(tAlu), mostrarAlu, NULL);
    return EXITO;
}

int testArbol()
{
    puts("\n==========================================================");
    puts("TESTEANDO LIBRERIA DE TDA ARBOL");
    puts("==========================================================");
    puts("");

    tArbol arbol;
    crearArbol(&arbol);
    int buffer;

    int valores[] = {50, 30, 70, 20, 40, 60, 80};

    printf(">>> Insertando valores...\n");
    for (size_t i = 0; i < sizeof(valores) / sizeof(*valores); ++i)
    {
        if (insArb(&arbol, valores+i, sizeof(int), cmpInt) == EXITO)
            printf("- Insertado: %d\n", *(valores+i));
        else
            printf("- Error al insertar: %d\n", *(valores+i));
    }

    printf("\n>>> Recorrido inorden:\n");
    recorrerIn(&arbol, mostrarInt, NULL);

    printf("\n>>> Recorrido preorden:\n");
    recorrerPre(&arbol, mostrarInt, NULL);

    printf("\n>>> Recorrido postorden:\n");
    recorrerPos(&arbol, mostrarInt, NULL);

    printf("\n>>> Altura del árbol: %zu\n", alturaArbol(&arbol));
    printf(">>> Cantidad de nodos: %zu\n", cantNodosArbol(&arbol));
    printf(">>> Cantidad de hojas: %zu\n", cantHojasArbol(&arbol));
    printf(">>> Cantidad de nodos no hoja: %zu\n", cantNoHojasArbol(&arbol));
    printf(">>> Es AVL: %s\n", esAVLArbol(&arbol) ? "Sí" : "No");

    busqElemMaxArbol(&arbol,&buffer,sizeof(int));
    printf("\n>>> Valor Maximo: %d\n",buffer);
    busqElemMinArbol(&arbol,&buffer,sizeof(int));
    printf(">>> Valor Minimo: %d\n",buffer);

    buffer = 40;
    printf("\n>>> Buscando valor %d:\n",buffer);
    if(busqElemArbol(&arbol,&buffer, sizeof(int),cmpInt) == EXITO)
        printf("- Valor encontrado\n");
    else
        printf("- Valor no encontrado\n");

    buffer = 30;
    printf("\n>>> Eliminar nodo de valor %d\n", buffer);
    if (eliminarNodo(&arbol, &buffer, cmpInt) == EXITO)
        printf("- Nodo %d eliminado.\n", buffer);
    else
        printf("- No se pudo eliminar el nodo %d.\n", buffer);

    printf("\n>>> Recorrido inorden luego de eliminar %d:\n", buffer);
    recorrerIn(&arbol, mostrarInt, NULL);

    eliminarArbol(&arbol);
    printf("\n>>> Arbol eliminado.\n");
}

int testArchivosArbol()
{
    puts("\n==========================================================");
    puts("TESTEANDO LIBRERIA DE TDA ARBOL CON ARCHIVOS GENERICOS");
    puts("==========================================================");
    puts("");

    FILE *arch;
    tArbol arbol;

    crearArbol(&arbol);

    printf(">>> Generando lote de alumnos...\n");
    if (generarLoteAlu(LOTE_ALUM) != EXITO)
    {
        printf("- Error al generar lote de alumnos.\n");
        return ERR_ARCH;
    }
    printf("- Archivo generado correctamente.\n");

    printf("\n>>> Leyendo todos los registros del archivo (%s):\n",LOTE_ALUM);
    mostrarAluCabe();
    leerArchBin(LOTE_ALUM, sizeof(tAlu), mostrarAlu, NULL);

    printf("\n>>> Cargando arbol desde archivo desordenado...\n");
    if (cargarArchBinDesArbol(LOTE_ALUM, &arbol, sizeof(tAlu), cmpAlu) != EXITO)
    {
        printf("- Error al cargar arbol desde archivo.\n");
        return ERR_INSER;
    }
    printf("- Arbol cargado exitosamente.\n");

    printf("\n>>> Recorrido inorden del arbol:\n");
    mostrarAluCabe();
    recorrerIn(&arbol, mostrarAlu, NULL);

    printf("\n>>> Guardando arbol en archivo ordenado (%s)...\n", LOTE_ALUM_ORD);
    if (crearArchBinArbol(LOTE_ALUM_ORD, &arbol) != EXITO)
    {
        printf("- ❌ Error al guardar árbol ordenado.\n");
        eliminarArbol(&arbol);
        return ERR_ARCH;
    }
    printf("- Árbol guardado correctamente en archivo ordenado.\n");

    eliminarArbol(&arbol);
    crearArbol(&arbol);

    printf("\n>>> Leyendo todos los registros del archivo (%s):\n",LOTE_ALUM_ORD);
    mostrarAluCabe();
    leerArchBin(LOTE_ALUM_ORD, sizeof(tAlu), mostrarAlu, NULL);

    printf("\n>>> Recargando árbol desde archivo ordenado...\n");
    if (cargarArchBinOrdArbol(LOTE_ALUM_ORD, &arbol, sizeof(tAlu)) != EXITO)
    {
        printf("- Error al cargar árbol ordenado.\n");
        return ERR_ARCH;
    }
    printf("- Árbol cargado desde archivo ordenado correctamente.\n");

    printf("\n>>> Recorrido inorden del árbol (balanceado):\n");
    mostrarAluCabe();
    recorrerIn(&arbol, mostrarAlu, NULL);

    eliminarArbol(&arbol);
    printf("\n>>> Arbol final eliminado.\n");

    return EXITO;
}

int testInd()
{
    puts("\n==========================================================");
    puts("TESTEANDO INDICES");
    puts("==========================================================");
    puts("");

    tAlu buffer;
    tIndCtx ctx;

    puts(">>> Generando lote de alumnos...\n");
    if(generarLoteAlu(LOTE_ALUM) != EXITO)
    {
        fprintf(stderr, "Error al generar lote de alumnos.\n");
        return ERR_ARCH;
    }

    printf(">>> Mostrando contenido del archivo original (%s):\n", LOTE_ALUM);
    mostrarAluCabe();
    leerArchBin(LOTE_ALUM, sizeof(tAlu), mostrarAlu, NULL);

    printf("\n>>> Creando Archivo Indice (%s)...\n", IND_ALUM);
    if(crearArchInd(LOTE_ALUM, IND_ALUM, sizeof(tAlu), cmpAlu, extraerClaveAlu) != EXITO)
    {
        fprintf(stderr, "Error al generar indice.\n");
        return ERR_ARCH;
    }

    printf("\n>>> Creando Contexto Indice...\n");
    if(inicializarIndCtx(&ctx, LOTE_ALUM, IND_ALUM, sizeof(tAlu), cmpAlu, extraerClaveAlu) != EXITO)
    {
        fprintf(stderr, "Error al generar indice.\n");
        return ERR_ARCH;
    }

    /// ------------------ NO SE DEBE HACER ESTO ------------------
    puts("\n>>> Mostrar arbol indice cargado...");
    mostrarIndCabe();
    recorrerIn(&ctx.arbol,mostrarInd,NULL);
    /// -----------------------------------------------------------

    puts("\n>>> Mostrando contenido del archivo original ordenado por el indice:");
    mostrarAluCabe();
    recorrerArchInd(&ctx, mostrarAlu, NULL);

    buffer.legajo = 1005;
    printf("\n>>> Buscar alumno con la clave %d:\n", buffer.legajo);
    if(busqArchInd(&ctx,&buffer) != EXITO)
        fprintf(stderr, "- Error, registro no encontrado.\n");
    else
    {
        printf("- Registro encontrado:\n");
        mostrarAluCabe();
        mostrarAlu(&buffer,NULL);
    }

    buffer.legajo = 1005;
    printf("\n>>> Eliminar registro con la clave %d\n",buffer.legajo);
    elimArchInd(&ctx, &buffer);

    buffer.legajo = 1030;
    printf("\n>>> Modificar al estado B del alumno con clave %d\n",buffer.legajo = 1030);
    modArchBinind(&ctx,&buffer,bajaAlu,NULL);

    buffer.legajo = 900;
    strcpy(buffer.nombre,"PATO");
    buffer.estado = 'A';
    *buffer.carrera = '\0';
    printf("\n>>> Insertar nuevo alumno con clave %d\n", buffer.legajo);
    insArchInd(&ctx,&buffer);

    puts("\n>>> Mostrando contenido del archivo original ordenado por el indice:");
    mostrarAluCabe();
    recorrerArchInd(&ctx,mostrarAlu,NULL);

    char est = 'B';
    printf("\n>>> Filtrar aquellos alumnos en el estado %c\n",est);
    filtrarArchInd(&ctx,filtroEstAlu,&est);

    puts("\n>>> Mostrando contenido del archivo original ordenado por el indice:");
    mostrarAluCabe();
    recorrerArchInd(&ctx,mostrarAlu,NULL);

    finalizarIndCtx(&ctx);
    return EXITO;
}
