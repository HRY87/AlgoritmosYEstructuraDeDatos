#ifndef PILAESTATICA_H_INCLUDED
#define PILAESTATICA_H_INCLUDED

#define MINIMO(X,Y)     ((X)<(Y)?(X):(Y))
#define TAM_PILA        1000 //CANTIDAD_DE_BYTES_DISPONIBLES

#define SIN_MEM         -1
#define TODO_OK         0
#define PILA_VACIA      1

typedef struct
{
    char        pila[TAM_PILA];
    unsigned    tope; //LIMITE_DISPONIBLE
}tPila;

void crearPila(tPila* pp);

int ponerEnPila(tPila* pp, const void* pd, unsigned tamDato);
int sacarDePila(tPila* pp, void* pd, unsigned tamDato);
int verTopePila(tPila* pp, void* pd, unsigned tamDato);

int pilaVacia(const tPila* pp);
int pilaLlena(const tPila* pp, unsigned tamDato);

void vaciarPila(tPila* pp);


#endif // PILAESTATICA_H_INCLUDED
