#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"

void impr_ent(void * i, FILE* pf)
{
    fprintf(pf, "%d ", *((int*)i));
}

void impr_pant_ent(void * i, unsigned n, void* pf)
{
    printf(" %d -", *((int*)i));
}

void imprimirConForma(void * info, unsigned tam, unsigned n, void * params)
{
    int * i = (int *) info;
    printf("%*s-%3d-\n",n*3,"", *i);
}

void imprimir(void * info, unsigned tam, unsigned n, void * params)
{
    int * i = (int *) info;
    printf("-%d*%d-", *i, n);
}

void escribir_arch_bin(void * info, unsigned tam, unsigned n, void * params)
{
    fwrite(info, tam, 1, (FILE*)params);
}

void mostrar_empleado(void * info, unsigned tam, unsigned n, void * params)
{
    t_empleado emp;
    t_reg_ind_alfa * reg_ind = (t_reg_ind_alfa *)info;
    FILE* pf = (FILE*)params;
    fseek(pf, reg_ind->nro_reg*sizeof(t_empleado), SEEK_SET);
    fread(&emp, sizeof(t_empleado), 1, pf);
    printf("\n");
    printf("%d - %s - %c - %f - %s", emp.dni, emp.apyn, emp.sexo, emp.sueldo, emp.info);
}

int leerDesdeVectorEnteros(void * info, void * vec, unsigned pos, size_t tam)
{
    memcpy(info, (int*)vec+pos, sizeof(int));
    return 1;
}

int leerDesdeVector(void * info, void * vec, unsigned pos, size_t tam)
{
    memcpy(info, vec+(tam*pos), tam);
    return 1;
}

int cmp_ent(const void *v1, const void *v2)
{
    return *((int*)v1)-*((int*)v2);
}

void imprimir_arch(void *info, unsigned tam, unsigned n, void *params)
{
    int * i = (int *) info;
    fprintf((FILE*) params,"-%d-", *i);
}

int crearArchivo(char*path, void * vec, unsigned tam, unsigned ce)
{
    FILE * pf = fopen(path, "wb");
    if (!pf)
        return 0;
    fwrite(vec, tam, ce, pf);
    fclose(pf);
    return 1;
}

void guardarVecEnteros(void *info, unsigned tam, unsigned n, void *params)
{
    struct s_vecRes * res = (struct s_vecRes*)params;
    int * infoEnt = (int*) info;

    res->vecRes[res->ce] = *infoEnt;

    res->ce++;

}

int cmp_dni_ind(const void *v1, const void *v2)
{
    t_reg_ind* r1 = (t_reg_ind*)v1, *r2 = (t_reg_ind*)v2;
    return r1->dni-r2->dni;
}

int cmp_alfa_ind(const void *v1, const void *v2)
{
    int comp;
    t_reg_ind_alfa* r1 = (t_reg_ind_alfa*)v1, *r2 = (t_reg_ind_alfa*)v2;
    return (comp=strcmpi(r1->apyn, r2->apyn))!=0?comp:r1->dni-r2->dni;
}
