#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colaEstatica.h"

void crearCola(tCola *cola)
{
    cola->pri=0;
    cola->ult=0;
    cola->tamDisp=TAM_COLA;
}

int colaVacia(const tCola *cola)
{
    if(cola->tamDisp==TAM_COLA)
        return COLA_VACIA;

    return TODO_OK;
}

void vaciarCola(tCola *cola)
{
    cola->pri=0;
    cola->ult=0;
    cola->tamDisp=TAM_COLA;
}

int colaLlena(const tCola *cola,unsigned tam)
{
    if(cola->tamDisp<tam+sizeof(unsigned))
        return COLA_LLENA;

    return TODO_OK;
}

int ponerEnCola(tCola *cola,const void* dato,unsigned tam)
{
    unsigned ini,fin;
    if(cola->tamDisp<tam+sizeof(unsigned))
        return 0;//Cola llena
    cola->tamDisp-=tam+sizeof(unsigned);
    //EL MINIMO ENTRE LO QUE QUIERO PONER Y LO QUE TENGO DISPONIBLE AL FINAL DE LA COLA
    ini=MINIMO(TAM_COLA-cola->ult, sizeof(unsigned));
    if(ini>0)
    {
        memcpy(cola->cola+cola->ult,&tam,ini);
    }
    fin=sizeof(unsigned)-ini;
    if(fin!=0)//SE TROZA EL DATO
    {
        //CASTEO A CHAR* PORQUE QUIERO QUE SE MUEVA CANTIDAD DE BYTES!!!!
        memcpy(cola->cola,(char*)&tam+ini,fin);
    }
    cola->ult=fin?fin:cola->ult+ini; //tambien puede ser cola->ult+sizeof(unsigned)


    //YA COPIE EL TAM DEL DATO POR QUE AHORA VUELVO A HACER TODO PARA LA INFO
    ini=MINIMO(TAM_COLA-cola->ult, tam);
    if(ini!=0)
    {
        memcpy(cola->cola+cola->ult,dato,ini);
    }
    fin=tam-ini;
    if(fin!=0)
        memcpy(cola->cola,(char*)dato+ini,fin);

    //ACOMODO EL ULTIMO
//    cola->ult=fin?fin:cola->ult+tam;
    cola->ult=fin?fin:cola->ult+ini;

    return TODO_OK;
}


int sacarDeCola(tCola *cola,void* dato,unsigned tam)
{
    unsigned ini,fin,tamInfo;
    if(cola->tamDisp==TAM_COLA)
        return COLA_VACIA;

    ini=MINIMO(TAM_COLA-cola->ult, sizeof(unsigned));

    if(ini!=0)
    {
        memcpy(&tamInfo,cola->cola+cola->pri,ini);
    }
    tamInfo=MINIMO(tamInfo,tam);
    fin=sizeof(unsigned)-ini;
    if(fin!=0)
    {
        memcpy((char*)&tamInfo+ini,cola->cola,fin);
    }
    //SI FIN ES DISTINTO DE 0, PONGO EL PRI EN EL LUGAR DE FIN, SINO SUMO
    cola->pri=fin?fin:cola->pri+sizeof(unsigned);
    ini=MINIMO(TAM_COLA-cola->pri,tamInfo);

    if(ini!=0)
    {
        memcpy(dato,cola->cola+cola->pri,ini);
    }
    fin=tamInfo-ini;
    if(fin!=0)
    {
        memcpy((char*)dato+ini,cola->cola,fin);
    }
    cola->pri=fin?fin:cola->pri+ini;
    cola->tamDisp+=tamInfo+sizeof(unsigned);

    return TODO_OK;
}

int verPrimeroCola (const tCola *cola,void* dato,unsigned tam)
{
    unsigned ini,fin,tamInfo;
    unsigned pos = cola->pri;

    if(cola->tamDisp==TAM_COLA)
        return COLA_VACIA;

    ini=MINIMO(TAM_COLA-cola->ult, sizeof(unsigned));

    if(ini!=0)
    {
        memcpy(&tamInfo,cola->cola+pos,ini);
    }
    fin=sizeof(unsigned)-ini;
    tamInfo=MINIMO(tam,tamInfo);
    if(fin!=0)
    {
        memcpy((char*)&tamInfo+ini,cola->cola,fin);
    }
    //PREGUNTO CON LA COPIA PARA NO MODIFICAR EL PUNTERO
    pos=fin?fin:pos+sizeof(unsigned);

    if(ini!=0)
    {
        memcpy(dato,cola->cola+pos,ini);
    }
    fin=tamInfo-ini;
    if(fin!=0)
    {
        memcpy((char*)dato+ini,cola->cola,fin);
    }

    return TODO_OK;
}
