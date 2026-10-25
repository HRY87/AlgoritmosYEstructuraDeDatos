#include <stdio.h>
#include <string.h>
#include "colaCircular.h"

#define TAM_NOMBRE 20
#define TAM_BUFFER 6

typedef struct
{
    int dni;
    char nombre[TAM_NOMBRE];
}tAlumno;

/** Pruebas **/
void pruebaColaVacia(void);
void pruebaEnterosFIFO(void);
void pruebaVerFrenteNoElimina(void);
void pruebaVaciar(void);
void pruebaReutilizarCola(void);
void pruebaIntercalado(void);
void pruebaBufferMenor(void);
void pruebaColaLlena(void);

/** Funciones de muestra (se pasan por puntero a funcion) **/
void mostrarEntero(const void* pd);
void mostrarAlumno(const void* pd);
void mostrarCadena(const void* pd);

/** Auxiliares **/
const char* textoResultado(int ret);
const char* textoSiNo(int valor);
int encolarVector(tCola* pc, const void* vec, size_t cantElem, size_t tamElem);
int desencolarMostrando(tCola* pc, void* buffer, unsigned tam, void (*mostrar)(const void*));


int main()
{
    pruebaColaVacia();
    pruebaEnterosFIFO();
    pruebaVerFrenteNoElimina();
    pruebaVaciar();
    pruebaReutilizarCola();
    pruebaIntercalado();
    pruebaBufferMenor();
    pruebaColaLlena();

    return 0;
}


/** PRUEBA 1: operaciones sobre cola vacia **/
void pruebaColaVacia(void)
{
    tCola cola;
    int frente, buffer, ret;

    printf("--- PRUEBA 1: operaciones sobre cola vacia ---\n");
    crearColaCircular(&cola);

    printf("Vacia?: %s\n", textoSiNo(colaCircularVacia(&cola)));
    ret = sacarColaCircular(&cola, &buffer, sizeof(int));
    printf("Sacar:     %s\n", textoResultado(ret));
    ret = verTopeColaCircular(&cola, &frente, sizeof(int));
    printf("Ver frente: %s\n\n", textoResultado(ret));
}

/** PRUEBA 2: poner enteros + ver frente + sacar (orden FIFO) **/
void pruebaEnterosFIFO(void)
{
    tCola cola;
    int vec[] = {10, 20, 30, 40, 50};
    int frente, buffer, ret;

    printf("--- PRUEBA 2: enteros (FIFO) ---\n");
    crearColaCircular(&cola);

    ret = encolarVector(&cola, vec, sizeof(vec)/sizeof(int), sizeof(int));
    printf("Encolar vector: %s\n", textoResultado(ret));

    ret = verTopeColaCircular(&cola, &frente, sizeof(int));
    printf("Ver frente: %s -> %d (esperado 10)\n", textoResultado(ret), frente);

    printf("Desencolando: ");
    ret = desencolarMostrando(&cola, &buffer, sizeof(int), mostrarEntero);
    printf("\n(esperado: 10 20 30 40 50)\n");
    printf("Cantidad desencolada: %d (esperado 5)\n", ret);
    printf("Vacia?: %s\n\n", textoSiNo(colaCircularVacia(&cola)));
}

/** PRUEBA 3: estructura, ver frente NO debe sacar el elemento **/
void pruebaVerFrenteNoElimina(void)
{
    tCola cola;
    tAlumno vec[] = {{1111, "Ana"}, {2222, "Beto"}, {3333, "Carla"}};
    tAlumno frente, buffer;

    printf("--- PRUEBA 3: estructura, ver frente no elimina ---\n");
    crearColaCircular(&cola);
    encolarVector(&cola, vec, sizeof(vec)/sizeof(tAlumno), sizeof(tAlumno));

    verTopeColaCircular(&cola, &frente, sizeof(tAlumno));
    printf("Ver frente (1ra vez): ");
    mostrarAlumno(&frente);

    verTopeColaCircular(&cola, &frente, sizeof(tAlumno));
    printf("\nVer frente (2da vez): ");
    mostrarAlumno(&frente);
    printf("\n(deben ser iguales, esperado Ana)\n");

    printf("Desencolando: ");
    desencolarMostrando(&cola, &buffer, sizeof(tAlumno), mostrarAlumno);
    printf("\n(esperado: Ana, Beto, Carla)\n\n");
}

/** PRUEBA 4: vaciar cola **/
void pruebaVaciar(void)
{
    tCola cola;
    int vec[] = {10, 20, 30, 40, 50};
    int frente, ret;

    printf("--- PRUEBA 4: vaciar cola ---\n");
    crearColaCircular(&cola);
    encolarVector(&cola, vec, sizeof(vec)/sizeof(int), sizeof(int));

    printf("Vacia antes de vaciar?:   %s\n", textoSiNo(colaCircularVacia(&cola)));
    vaciarColaCircular(&cola);
    printf("Vacia despues de vaciar?: %s\n", textoSiNo(colaCircularVacia(&cola)));

    ret = verTopeColaCircular(&cola, &frente, sizeof(int));
    printf("Ver frente tras vaciar: %s\n\n", textoResultado(ret));
}

/** PRUEBA 5: reutilizar la cola luego de vaciarla **/
void pruebaReutilizarCola(void)
{
    tCola cola;
    int vec[] = {10, 20, 30};
    int frente, ret;

    printf("--- PRUEBA 5: reutilizar cola tras vaciar ---\n");
    crearColaCircular(&cola);
    encolarVector(&cola, vec, sizeof(vec)/sizeof(int), sizeof(int));
    vaciarColaCircular(&cola);

    ponerColaCircular(&cola, vec + 1, sizeof(int));
    ret = verTopeColaCircular(&cola, &frente, sizeof(int));
    printf("Ver frente: %s -> %d (esperado 20)\n\n", textoResultado(ret), frente);

    vaciarColaCircular(&cola);
}

/** PRUEBA 6: alternar poner y sacar hasta quedar vacia (caso de un solo nodo) **/
void pruebaIntercalado(void)
{
    tCola cola;
    int vec[] = {10, 20, 30};
    int buffer, ret;

    printf("--- PRUEBA 6: poner y sacar intercalado ---\n");
    crearColaCircular(&cola);

    ponerColaCircular(&cola, vec, sizeof(int));
    ponerColaCircular(&cola, vec + 1, sizeof(int));

    ret = sacarColaCircular(&cola, &buffer, sizeof(int));
    printf("Sacar: %s -> %d (esperado 10)\n", textoResultado(ret), buffer);

    ponerColaCircular(&cola, vec + 2, sizeof(int));

    ret = sacarColaCircular(&cola, &buffer, sizeof(int));
    printf("Sacar: %s -> %d (esperado 20)\n", textoResultado(ret), buffer);

    ret = sacarColaCircular(&cola, &buffer, sizeof(int));
    printf("Sacar: %s -> %d (esperado 30)\n", textoResultado(ret), buffer);

    ret = sacarColaCircular(&cola, &buffer, sizeof(int));
    printf("Sacar con cola ya vacia: %s\n", textoResultado(ret));
    printf("Vacia?: %s\n", textoSiNo(colaCircularVacia(&cola)));

    ponerColaCircular(&cola, vec, sizeof(int));
    ret = verTopeColaCircular(&cola, &buffer, sizeof(int));
    printf("Reuso: %s -> %d (esperado 10)\n\n", textoResultado(ret), buffer);

    vaciarColaCircular(&cola);
}

/** PRUEBA 7: buffer menor al dato guardado (usa MINIMO) **/
void pruebaBufferMenor(void)
{
    tCola cola;
    char cadena[] = "Hola mundo";
    char buffer[TAM_BUFFER] = {0};
    int ret;

    printf("--- PRUEBA 7: buffer menor al dato guardado ---\n");
    crearColaCircular(&cola);

    ponerColaCircular(&cola, cadena, strlen(cadena) + 1);
    ret = sacarColaCircular(&cola, buffer, TAM_BUFFER - 1);

    printf("Sacar: %s -> \"", textoResultado(ret));
    mostrarCadena(buffer);
    printf("\" (esperado: los primeros %d caracteres)\n\n", TAM_BUFFER - 1);

    vaciarColaCircular(&cola);
}

/** PRUEBA 8: cola llena (consulta por memoria disponible) **/
void pruebaColaLlena(void)
{
    tCola cola;

    printf("--- PRUEBA 8: cola llena ---\n");
    crearColaCircular(&cola);

    printf("Llena para un int?: %s\n\n", textoSiNo(colaCircularLlena(&cola, sizeof(int))));
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
    static const char* textos[] = {"SIN_MEM", "TODO_OK", "COLA_VACIA"};

    return *(textos + ret + 1);
}

const char* textoSiNo(int valor)
{
    static const char* textos[] = {"NO", "SI"};

    return *(textos + (valor != 0));
}

/* Encola un vector de cualquier tipo avanzando con aritmetica de punteros */
int encolarVector(tCola* pc, const void* vec, size_t cantElem, size_t tamElem)
{
    const char* ini = (const char*)vec;
    const char* fin = ini + cantElem * tamElem;
    int ret = TODO_OK;

    while(ini < fin && ret == TODO_OK)
    {
        ret = ponerColaCircular(pc, ini, tamElem);
        ini += tamElem;
    }

    return ret;
}

/* Saca todos los elementos y los muestra con la funcion recibida */
int desencolarMostrando(tCola* pc, void* buffer, unsigned tam, void (*mostrar)(const void*))
{
    int cant = 0;

    while(sacarColaCircular(pc, buffer, tam) == TODO_OK)
    {
        mostrar(buffer);
        cant++;
    }

    return cant;
}
