#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

void impr_ent(void * i, FILE* pf);
void impr_pant_ent(void * i, unsigned n, void* pf);

int cmp_ent(const void *v1, const void *v2);
int cmp_dni_ind(const void *v1, const void *v2);
int cmp_alfa_ind(const void *v1, const void *v2);

void imprimirConForma(void *info, unsigned tam, unsigned n, void *params);
void imprimir(void *info, unsigned tam, unsigned n, void *params);
void imprimir_arch(void *info, unsigned tam, unsigned n, void *params);
void guardarVecEnteros(void *info, unsigned tam, unsigned n, void *params);
void escribir_arch_bin(void * info, unsigned tam, unsigned n, void * params);
void mostrar_empleado(void * info, unsigned tam, unsigned n, void * params);

int leerDesdeVectorEnteros(void* info, void *vec, unsigned pos, size_t tam);
int leerDesdeVector(void* info, void * vec, unsigned pos, size_t tam);

int crearArchivo(char*path, void * vec, unsigned tam, unsigned ce);

typedef struct
{
    int dni;
    char apyn[20];
    char sexo;
    float sueldo;
    char info[500];
}t_empleado;

typedef struct
{
    int dni;
    int nro_reg;
}t_reg_ind;

typedef struct
{
    int dni;
    char apyn[20];
    int nro_reg;
}t_reg_ind_alfa;

struct s_vecRes
{
    int vecRes[12];
    int ce;
};
#endif // UTILIDADES_H_INCLUDED
