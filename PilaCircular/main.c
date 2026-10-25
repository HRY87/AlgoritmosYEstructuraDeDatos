#include <stdio.h>
#include <string.h>
#include "pilaCircular.h"

#define TAM_NOMBRE 20
#define TAM_BUFFER 6

typedef struct
{
    int dni;
    char nombre[TAM_NOMBRE];
}tAlumno;

/** Pruebas **/
void pruebaPilaVacia(void);
void pruebaEnterosLIFO(void);
void pruebaVerTopeNoElimina(void);
void pruebaVaciar(void);
void pruebaReutilizarPila(void);
void pruebaBufferMenor(void);
void pruebaPilaLlena(void);

/** Funciones de muestra (se pasan por puntero a funcion) **/
void mostrarEntero(const void* pd);
void mostrarAlumno(const void* pd);
void mostrarCadena(const void* pd);

/** Auxiliares **/
const char* textoResultado(int ret);
const char* textoSiNo(int valor);
int apilarVector(tPila* pp, const void* vec, size_t cantElem, size_t tamElem);
int desapilarMostrando(tPila* pp, void* buffer, unsigned tam, void (*mostrar)(const void*));


int main()
{
    pruebaPilaVacia();
    pruebaEnterosLIFO();
    pruebaVerTopeNoElimina();
    pruebaVaciar();
    pruebaReutilizarPila();
    pruebaBufferMenor();
    pruebaPilaLlena();

    return 0;
}


/** PRUEBA 1: operaciones sobre pila vacia **/
void pruebaPilaVacia(void)
{
    tPila pila;
    int tope, buffer, ret;

    printf("--- PRUEBA 1: operaciones sobre pila vacia ---\n");
    crearPilaCircular(&pila);

    printf("Vacia?: %s\n", textoSiNo(pilaCircularVacia(&pila)));
    ret = sacarPilaCircular(&pila, &buffer, sizeof(int));
    printf("Sacar:    %s\n", textoResultado(ret));
    ret = verTopePilaCircular(&pila, &tope, sizeof(int));
    printf("Ver tope: %s\n\n", textoResultado(ret));
}

/** PRUEBA 2: poner enteros + ver tope + sacar (orden LIFO) **/
void pruebaEnterosLIFO(void)
{
    tPila pila;
    int vec[] = {10, 20, 30, 40, 50};
    int tope, buffer, ret;

    printf("--- PRUEBA 2: enteros (LIFO) ---\n");
    crearPilaCircular(&pila);

    ret = apilarVector(&pila, vec, sizeof(vec)/sizeof(int), sizeof(int));
    printf("Apilar vector: %s\n", textoResultado(ret));

    ret = verTopePilaCircular(&pila, &tope, sizeof(int));
    printf("Ver tope: %s -> %d (esperado 50)\n", textoResultado(ret), tope);

    printf("Desapilando: ");
    ret = desapilarMostrando(&pila, &buffer, sizeof(int), mostrarEntero);
    printf("\nCantidad desapilada: %d (esperado 5)\n", ret);
    printf("Vacia?: %s\n\n", textoSiNo(pilaCircularVacia(&pila)));
}

/** PRUEBA 3: estructura, ver tope NO debe sacar el elemento **/
void pruebaVerTopeNoElimina(void)
{
    tPila pila;
    tAlumno vec[] = {{1111, "Ana"}, {2222, "Beto"}, {3333, "Carla"}};
    tAlumno tope, buffer;

    printf("--- PRUEBA 3: estructura, ver tope no elimina ---\n");
    crearPilaCircular(&pila);
    apilarVector(&pila, vec, sizeof(vec)/sizeof(tAlumno), sizeof(tAlumno));

    verTopePilaCircular(&pila, &tope, sizeof(tAlumno));
    printf("Ver tope (1ra vez): ");
    mostrarAlumno(&tope);

    verTopePilaCircular(&pila, &tope, sizeof(tAlumno));
    printf("\nVer tope (2da vez): ");
    mostrarAlumno(&tope);
    printf("\n(deben ser iguales)\n");

    printf("Desapilando: ");
    desapilarMostrando(&pila, &buffer, sizeof(tAlumno), mostrarAlumno);
    printf("\n\n");
}

/** PRUEBA 4: vaciar pila **/
void pruebaVaciar(void)
{
    tPila pila;
    int vec[] = {10, 20, 30, 40, 50};
    int tope, ret;

    printf("--- PRUEBA 4: vaciar pila ---\n");
    crearPilaCircular(&pila);
    apilarVector(&pila, vec, sizeof(vec)/sizeof(int), sizeof(int));

    printf("Vacia antes de vaciar?:   %s\n", textoSiNo(pilaCircularVacia(&pila)));
    vaciarPilaCircular(&pila);
    printf("Vacia despues de vaciar?: %s\n", textoSiNo(pilaCircularVacia(&pila)));

    ret = verTopePilaCircular(&pila, &tope, sizeof(int));
    printf("Ver tope tras vaciar: %s\n\n", textoResultado(ret));
}

/** PRUEBA 5: reutilizar la pila luego de vaciarla **/
void pruebaReutilizarPila(void)
{
    tPila pila;
    int vec[] = {10, 20, 30};
    int tope, ret;

    printf("--- PRUEBA 5: reutilizar pila tras vaciar ---\n");
    crearPilaCircular(&pila);
    apilarVector(&pila, vec, sizeof(vec)/sizeof(int), sizeof(int));
    vaciarPilaCircular(&pila);

    ponerPilaCircular(&pila, vec, sizeof(int));
    ret = verTopePilaCircular(&pila, &tope, sizeof(int));
    printf("Ver tope: %s -> %d (esperado 10)\n\n", textoResultado(ret), tope);

    vaciarPilaCircular(&pila);
}

/** PRUEBA 6: buffer menor al dato guardado (usa MINIMO) **/
void pruebaBufferMenor(void)
{
    tPila pila;
    char cadena[] = "Hola mundo";
    char buffer[TAM_BUFFER] = {0};
    int ret;

    printf("--- PRUEBA 6: buffer menor al dato guardado ---\n");
    crearPilaCircular(&pila);

    ponerPilaCircular(&pila, cadena, strlen(cadena) + 1);
    ret = sacarPilaCircular(&pila, buffer, TAM_BUFFER - 1);

    printf("Sacar: %s -> \"", textoResultado(ret));
    mostrarCadena(buffer);
    printf("\" (esperado: los primeros %d caracteres)\n\n", TAM_BUFFER - 1);

    vaciarPilaCircular(&pila);
}

/** PRUEBA 7: pila llena (consulta por memoria disponible) **/
void pruebaPilaLlena(void)
{
    tPila pila;

    printf("--- PRUEBA 7: pila llena ---\n");
    crearPilaCircular(&pila);

    printf("Llena para un int?: %s\n\n", textoSiNo(pilaCircularLlena(&pila, sizeof(int))));
}


/** ---------- Funciones de muestra ---------- **/

void mostrarEntero(const void* pd)
{
    printf("%d ", *(const int*)pd);
}

void mostrarAlumno(const void* pd)
{
    const tAlumno* alu = (const tAlumno*)pd;

    printf("[%d - %s] ", alu->dni, alu->nombre);
}

void mostrarCadena(const void* pd)
{
    printf("%s", (const char*)pd);
}


/** ---------- Auxiliares ---------- **/

const char* textoResultado(int ret)
{
    static const char* textos[] = {"SIN_MEM", "TODO_OK", "PILA_VACIA"};

    return *(textos + ret + 1);
}

const char* textoSiNo(int valor)
{
    static const char* textos[] = {"NO", "SI"};

    return *(textos + (valor != 0));
}

/* Apila un vector de cualquier tipo avanzando con aritmetica de punteros */
int apilarVector(tPila* pp, const void* vec, size_t cantElem, size_t tamElem)
{
    const char* ini = (const char*)vec;
    const char* fin = ini + cantElem * tamElem;
    int ret = TODO_OK;

    while(ini < fin && ret == TODO_OK)
    {
        ret = ponerPilaCircular(pp, ini, tamElem);
        ini += tamElem;
    }

    return ret;
}

/* Saca todos los elementos y los muestra con la funcion recibida */
int desapilarMostrando(tPila* pp, void* buffer, unsigned tam, void (*mostrar)(const void*))
{
    int cant = 0;

    while(sacarPilaCircular(pp, buffer, tam) == TODO_OK)
    {
        mostrar(buffer);
        cant++;
    }

    return cant;
}
