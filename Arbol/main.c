#include <stdio.h>
#include <stdlib.h>
#include "utilidades.h"
#include "arbolBusqueda.h"

void probarFuncionesVarias();
void probarArbol();

void probarIndiceCrear();
void probarIndiceFuncionamiento();

int main()
{
    //probarFuncionesVarias();
    probarArbol();
    //probarIndiceCrear();
    //probarIndiceFuncionamiento();

    return 0;
}

void probarIndiceCrear()
{
    FILE * pf;
    int i=0;
    tArbolBinBusq indice;
    tArbolBinBusq indice_alfa;
    t_reg_ind reg_ind;
    t_empleado empleados[] = {
        {12345678, "Juan Perez", 'M', 2500.0, "Informacion sobre Juan Perez"},
        {23456789, "Maria Gomez", 'F', 2800.0, "Informacion sobre Maria Gomez"},
        {78901234, "Diego Gonzalez", 'M', 2600.0, "Informacion sobre Diego Gonzalez"},
        {56789012, "Pedro Sanchez", 'M', 2700.0, "Informacion sobre Pedro Sanchez"},
        {90123456, "Javier Perez", 'M', 2800.0, "Informacion sobre Javier Perez"},
        {45678901, "Ana Rodriguez", 'F', 3200.0, "Informacion sobre Ana Rodriguez"},
        {67890123, "Laura Lopez", 'F', 2900.0, "Informacion sobre Laura Lopez"},
        {10111213, "Lucia Martinez", 'F', 3000.0, "Informacion sobre Lucia Martinez"},
        {89012345, "Silvia Fernandez", 'F', 3100.0, "Informacion sobre Silvia Fernandez"},
        {34567890, "Carlos Martinez", 'M', 3000.0, "Informacion sobre Carlos Martinez"}
    }, emp;

    crearArchivo("empleados.dat", empleados, sizeof(t_empleado), sizeof(empleados)/sizeof(t_empleado));


    crearArbolBinBusq(&indice);
    crearArbolBinBusq(&indice_alfa);

    if ((pf=fopen("empleados.dat", "rb"))==NULL)
        return;

    fread(&emp, sizeof(t_empleado), 1, pf);
    while(!feof(pf))
    {
        t_reg_ind r = {emp.dni, i};
        t_reg_ind_alfa ra= {emp.dni, "", i};
        strcpy(ra.apyn, emp.apyn);
        i++;
        insertarArbolBinBusq(&indice, &r, sizeof(t_reg_ind), cmp_dni_ind);
        insertarArbolBinBusq(&indice_alfa, &ra, sizeof(t_reg_ind_alfa), cmp_alfa_ind);
        fread(&emp, sizeof(t_empleado), 1, pf);
    }

    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice, NULL, imprimirConForma);

    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice_alfa, NULL, imprimirConForma);

    reg_ind.dni = 45678901;

    buscarElemArbolBinBusq(&indice, &reg_ind, sizeof(t_reg_ind), cmp_dni_ind);

    fseek(pf, reg_ind.nro_reg*sizeof(t_empleado), SEEK_SET);
    fread(&emp, sizeof(t_empleado), 1, pf);
    printf("\n");
    printf("%d - %s - %c - %f - %s", emp.dni, emp.apyn, emp.sexo, emp.sueldo, emp.info);

    fclose(pf);

    ///Guardo los indices
    if ((pf=fopen("empleados.idx", "wb"))==NULL)
        return;

    recorrerEnOrdenArbolBinBusq(&indice, pf, escribir_arch_bin);
    fclose(pf);

    if ((pf=fopen("empleados_alfa.idx", "wb"))==NULL)
        return;

    recorrerEnOrdenArbolBinBusq(&indice_alfa, pf, escribir_arch_bin);
    fclose(pf);
}


void probarIndiceFuncionamiento()
{
    FILE * pf;
    tArbolBinBusq indice;
    tArbolBinBusq indice_alfa;
    t_reg_ind reg_ind;
    t_reg_ind_alfa reg_ind_alfa;
    t_empleado emp, new_emp ={35555555, "Juan Martinez", 'M', 3000.0, "Informaciin sobre Juan Martinez"};

    if ((pf=fopen("empleados.dat", "r+b"))==NULL)
        return;

    crearArbolBinBusq(&indice);
    crearArbolBinBusq(&indice_alfa);
    cargarArchivoBinOrdenadoArbolBinBusq(&indice, "empleados.idx", sizeof(t_reg_ind));
    cargarArchivoBinOrdenadoArbolBinBusq(&indice_alfa, "empleados_alfa.idx", sizeof(t_reg_ind_alfa));
    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice, NULL, imprimirConForma);

    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice_alfa, NULL, imprimirConForma);

    reg_ind.dni = 45678901;

    buscarElemArbolBinBusq(&indice, &reg_ind, sizeof(t_reg_ind), cmp_dni_ind);

    fseek(pf, reg_ind.nro_reg*sizeof(t_empleado), SEEK_SET);
    fread(&emp, sizeof(t_empleado), 1, pf);
    printf("\n");
    printf("%d - %s - %c - %f - %s", emp.dni, emp.apyn, emp.sexo, emp.sueldo, emp.info);

    fseek(pf, 0l, SEEK_END);
    reg_ind.nro_reg = ftell(pf)/sizeof(t_empleado);
    reg_ind.dni = new_emp.dni;
    fwrite(&new_emp, sizeof(t_empleado), 1, pf);
    insertarArbolBinBusq(&indice, &reg_ind, sizeof(t_reg_ind), cmp_dni_ind);
    reg_ind_alfa.nro_reg = reg_ind.nro_reg;
    strcpy(reg_ind_alfa.apyn, new_emp.apyn);
    reg_ind_alfa.dni = new_emp.dni;
    insertarArbolBinBusq(&indice_alfa, &reg_ind_alfa, sizeof(t_reg_ind_alfa), cmp_alfa_ind);

    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice, NULL, imprimirConForma);

    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&indice_alfa, NULL, imprimirConForma);

    reg_ind.nro_reg=0;
    reg_ind.dni = new_emp.dni;
    buscarElemArbolBinBusq(&indice, &reg_ind, sizeof(t_reg_ind), cmp_dni_ind);
    fseek(pf, reg_ind.nro_reg*sizeof(t_empleado), SEEK_SET);
    fread(&emp, sizeof(t_empleado), 1, pf);
    printf("\n");
    printf("%d - %s - %c - %f - %s", emp.dni, emp.apyn, emp.sexo, emp.sueldo, emp.info);

    printf("\n");
    printf("\nRegistros ordenados alfabeticamente");
    printf("\n");
    recorrerEnOrdenArbolBinBusq(&indice_alfa, pf, mostrar_empleado);


    fclose(pf);

    ///Guardo los indices
    if ((pf=fopen("empleados.idx", "wb"))==NULL)
        return;

    recorrerEnOrdenArbolBinBusq(&indice, pf, escribir_arch_bin);
    fclose(pf);

    if ((pf=fopen("empleados_alfa.idx", "wb"))==NULL)
        return;

    recorrerEnOrdenArbolBinBusq(&indice_alfa, pf, escribir_arch_bin);
    fclose(pf);
}

void probarArbol()
{
    FILE * pf;
    int i, dato, elim = 100;
    //int vec[] = {100, 50, 150, 25, 75, 125, 175, 10, 30, 60, 110, 130, 160, 200, 55, 70};
    int vec[] = { 10, 25, 50, 60, 75, 100, 110, 125, 150, 175, 200};
    //int vec[] = {100, 50, 25, 10, 30, 75, 60, 55, 70, 150, 125, 110, 130, 175, 160, 200};
    tArbolBinBusq arbol;
    crearArbolBinBusq(&arbol);

    for (i =0; i<sizeof(vec)/sizeof(int); i++)
    {
        insertarArbolBinBusq(&arbol, vec+i, sizeof(int), cmp_ent);
    }

    crearArchivo("ordenado.dat", vec, sizeof(int),sizeof(vec)/sizeof(int));

    cargarArchivoBinOrdenadoArbolBinBusq(&arbol,
                                         "ordenado.dat",
                                         sizeof(int));

//    cargarDesdeDatosOrdenadosArbolBinBusq(&arbol,
//                                          vec,
//                                          sizeof(vec)/sizeof(int),
//                                          leerDesdeVector,
//                                          sizeof(int));
//
    recorrerEnOrdenSimpleArbolBinBusq(&arbol, NULL, impr_pant_ent);
    printf("\n");
    printf("\n");
    printf("\n");
    recorrerEnOrdenInversoArbolBinBusq(&arbol, NULL, imprimirConForma);
//    recorrerPreOrdenSimpleArbolBinBusq(&arbol, NULL, impr_pant_ent);
//
//    printf("\n");
//    printf("\n");
//    recorrerEnOrdenInversoArbolBinBusq(&arbol, NULL, imprimirConForma);
//
//    printf("\n");
//    printf("\n");
//    dato = alturaArbolBin(&arbol);
//    printf("altura=%d", dato);
//
//    eliminarElemArbolBinBusq(&arbol, &elim, sizeof(elim), cmp_ent);
//    printf("\n");
//    printf("\n");
//    recorrerEnOrdenInversoArbolBinBusq(&arbol, NULL, imprimirConForma);
//
//
//    printf("\n\n");
//    if (esCompletoArbolBin(&arbol))
//        printf("\nCompleto");
//    if (esBalanceadoArbolBin(&arbol))
//        printf("\nBalanceado");
//    if (esAVLArbolBin(&arbol))
//        printf("\nAVL");
//
//    printf("\n\n");
//    if (esCompleto2ArbolBin(&arbol))
//        printf("\n2 Completo");
//    if (esBalanceado2ArbolBin(&arbol))
//        printf("\n2 Balanceado");
//    if (esAVL2ArbolBin(&arbol))
//        printf("\n2 AVL");
//
//    if ((pf=fopen("arbol.dat", "wb"))==NULL)
//        return;
//
//    recorrerEnOrdenArbolBinBusq(&arbol, pf, escribir_arch_bin);
//    fclose(pf);
}

void probarFuncionesVarias()
{
    tArbolBinBusq ar;
    int vec[] = {10,25,50,60,75,100,110,125,150,175,200},
                tamVec = sizeof(vec)/sizeof(int);
    //int vec[] = {25,50,75,100,125,150,175};
    int vecPre []= {100, 50, 25, 10, 75, 150, 125},
                   tamVecPre = sizeof(vecPre)/sizeof(int),
                   i;

    struct s_vecRes res= {{0}, 0};

    unsigned tamDato = sizeof(int);
    int dato;
    FILE * pf;

    crearArchivo("ordenado.dat", vec, sizeof(int),sizeof(vec)/sizeof(int));

    crearArbolBinBusq(&ar);

//    cargarArchivoBinOrdenadoArbolBinBusq(&ar, "ordenado.dat", sizeof(int));

    cargarDesdeDatosOrdenadosArbolBinBusq(&ar,
                                          vec,
                                          sizeof(vec)/sizeof(int),
                                          leerDesdeVector,
                                          tamDato);

    ///eliminarRaizArbolBinBusq(&ar);

    dato = alturaArbolBin(&ar);

    for(i=0; i< tamVecPre; i++)
    {
        insertarArbolBinBusq(&ar, vecPre+i, sizeof(int), cmp_ent);
    }

    for(i=0; i< tamVec; i++)
    {
        insertarArbolBinBusq(&ar, vec+i, sizeof(int), cmp_ent);
    }

    ///cargar el arbol en un archivo de texto
    pf = fopen("arbol.txt", "wt");
    if(!pf)
        return;

    recorrerEnOrdenArbolBinBusq(&ar,pf,imprimir_arch);
    fclose(pf);

    ///cargar el arbol en un vector de enteros
    recorrerEnOrdenArbolBinBusq(&ar,&res,guardarVecEnteros);
    for(i=0; i<res.ce; i++)
    {
        printf("-%d-", res.vecRes[i]);
    }
    printf("\n\n");

    ///imprimir el arbol
    recorrerEnOrdenArbolBinBusq(&ar,NULL,imprimir);
    printf("\n\n");
    recorrerPreOrdenArbolBinBusq(&ar,NULL,imprimir);
    printf("\n\n");
    recorrerPosOrdenArbolBinBusq(&ar,NULL,imprimir);
    printf("\n\n");
    recorrerEnOrdenInversoArbolBinBusq(&ar,NULL,imprimirConForma);

    printf("\n\n");
    if (esCompletoArbolBin(&ar))
        printf("\nCompleto");
    if (esBalanceadoArbolBin(&ar))
        printf("\nBalanceado");
    if (esAVLArbolBin(&ar))
        printf("\nAVL");

    printf("\n\n");
    if (esCompleto2ArbolBin(&ar))
        printf("\n2 Completo");
    if (esBalanceado2ArbolBin(&ar))
        printf("\n2 Balanceado");
    if (esAVL2ArbolBin(&ar))
        printf("\n2 AVL");

    mayorElemNoClaveArbolBinBusq(&ar, &dato, sizeof(dato),cmp_ent);
    printf("\n\nMayor No Clave: %d", dato);

    menorElemNoClaveArbolBinBusq(&ar, &dato, sizeof(dato),cmp_ent);
    printf("\n\nMenor No Clave: %d", dato);
}
