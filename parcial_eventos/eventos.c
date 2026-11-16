#include "eventos.h"

#define FORMATO_ALERTA  "%d|%08d|%06d|%08d|%06d\n"

#define ANIO(f) ((f) / 10000)
#define MES(f)  (((f) / 100) % 100)
#define DIA(f)  ((f) % 100)
#define HH(h)   ((h) / 10000)
#define MM(h)   (((h) / 100) % 100)
#define SS(h)   ((h) % 100)

/** Estado del dispositivo que se esta procesando (el archivo esta ordenado por dispositivo) **/
typedef struct
{
    int codDisp;
    int cantSeguidas; //Alarmas criticas consecutivas guardadas en la cola
    int critico;      //El dispositivo ya genero su alerta
    tCola secuencia;  //Cola circular: el primero es el inicio de la secuencia
}tMonitor;

static void iniciarMonitor(tMonitor* mon);
static void reiniciarSecuencia(tMonitor* mon);
static void reiniciarMonitor(tMonitor* mon, int codDisp);
static int acumularAlarma(tMonitor* mon, const tEvento* ev, FILE* pfSal);
static int procesarEvento(tMonitor* mon, const tEvento* ev, FILE* pfSal);
static int generarAlertas(FILE* pfEnt, FILE* pfSal);


int parsearEvento(const char* linea, tEvento* ev)
{
    int campos = sscanf(linea, "%d|%d|%d|%d", &ev->codDisp, &ev->fecha, &ev->hora, &ev->alarma);

    return (campos == 4 && ev->alarma >= NIVEL_MIN && ev->alarma <= NIVEL_MAX) ? TODO_OK : EVENTO_INVALIDO;
}

int procesarEventos(const char* pathEnt, const char* pathSal)
{
    FILE* pfEnt = fopen(pathEnt, "rt");
    FILE* pfSal;
    int cant;

    if(!pfEnt)
        return ERROR_ARCH;

    pfSal = fopen(pathSal, "wt");

    if(!pfSal)
    {
        fclose(pfEnt);
        return ERROR_ARCH;
    }

    cant = generarAlertas(pfEnt, pfSal);

    fclose(pfEnt);
    fclose(pfSal);

    return cant;
}

static int generarAlertas(FILE* pfEnt, FILE* pfSal)
{
    char linea[TAM_LINEA];
    tEvento ev;
    tMonitor mon;
    int cant = 0,
        ret = TODO_OK;

    iniciarMonitor(&mon);

    while(ret != SIN_MEM && fgets(linea, sizeof(linea), pfEnt))
    {
        if(parsearEvento(linea, &ev) == TODO_OK)
        {
            ret = procesarEvento(&mon, &ev, pfSal);
            cant += (ret == ALERTA_GENERADA);
        }
    }

    /**Libera los nodos que pudieron quedar si el archivo termino en medio de una secuencia**/
    reiniciarSecuencia(&mon);

    return (ret == SIN_MEM) ? SIN_MEM : cant;
}

static int procesarEvento(tMonitor* mon, const tEvento* ev, FILE* pfSal)
{
    int ret = TODO_OK;

    /**Cambio de dispositivo: se descarta la secuencia del anterior**/
    if(ev->codDisp != mon->codDisp)
        reiniciarMonitor(mon, ev->codDisp);

    if(ev->alarma != NIVEL_CRITICO)
        reiniciarSecuencia(mon); //Se corta la racha de alarmas consecutivas
    else if(!mon->critico)
        ret = acumularAlarma(mon, ev, pfSal);

    return ret;
}

static int acumularAlarma(tMonitor* mon, const tEvento* ev, FILE* pfSal)
{
    tEvento primero;
    int ret = ponerColaCircular(&mon->secuencia, ev, sizeof(tEvento));

    if(ret == TODO_OK)
    {
        mon->cantSeguidas++;

        if(mon->cantSeguidas == CANT_CRITICA)
        {
            verTopeColaCircular(&mon->secuencia, &primero, sizeof(tEvento));

            fprintf(pfSal, FORMATO_ALERTA, ev->codDisp, primero.fecha, primero.hora, ev->fecha, ev->hora);

            reiniciarSecuencia(mon);
            mon->critico = 1;
            ret = ALERTA_GENERADA;
        }
    }

    return ret;
}

static void iniciarMonitor(tMonitor* mon)
{
    crearColaCircular(&mon->secuencia);
    mon->codDisp = 0;
    mon->cantSeguidas = 0;
    mon->critico = 0;
}

static void reiniciarSecuencia(tMonitor* mon)
{
    vaciarColaCircular(&mon->secuencia);
    mon->cantSeguidas = 0;
}

static void reiniciarMonitor(tMonitor* mon, int codDisp)
{
    reiniciarSecuencia(mon);
    mon->codDisp = codDisp;
    mon->critico = 0;
}


/** ---------- Mostrar archivos ---------- **/

int mostrarArchivo(const char* path, const char* titulo, void (*mostrar)(const char*))
{
    FILE* pf = fopen(path, "rt");
    char linea[TAM_LINEA];
    int cant = 0;

    if(!pf)
        return ERROR_ARCH;

    printf("\n=== %s (%s) ===\n", titulo, path);
    mostrar(NULL);

    while(fgets(linea, sizeof(linea), pf))
    {
        *(linea + strcspn(linea, "\r\n")) = '\0';
        mostrar(linea);
        cant++;
    }

    fclose(pf);

    return cant;
}

void mostrarEvento(const char* linea)
{
    tEvento ev;

    if(!linea)
        printf("%-12s %-12s %-10s %s\n", "Dispositivo", "Fecha", "Hora", "Alarma");
    else if(parsearEvento(linea, &ev) == TODO_OK)
        printf("%-12d %02d/%02d/%04d   %02d:%02d:%02d   %d\n",
               ev.codDisp, DIA(ev.fecha), MES(ev.fecha), ANIO(ev.fecha),
               HH(ev.hora), MM(ev.hora), SS(ev.hora), ev.alarma);
    else
        printf("%s  <-- linea invalida (se ignora)\n", linea);
}

void mostrarAlerta(const char* linea)
{
    int cod, fIni, hIni, fFin, hFin;

    if(!linea)
        printf("%-12s %-22s %s\n", "Dispositivo", "Primera alarma", "Tercera alarma");
    else if(sscanf(linea, "%d|%d|%d|%d|%d", &cod, &fIni, &hIni, &fFin, &hFin) == 5)
        printf("%-12d %02d/%02d/%04d %02d:%02d:%02d   %02d/%02d/%04d %02d:%02d:%02d\n",
               cod,
               DIA(fIni), MES(fIni), ANIO(fIni), HH(hIni), MM(hIni), SS(hIni),
               DIA(fFin), MES(fFin), ANIO(fFin), HH(hFin), MM(hFin), SS(hFin));
    else
        printf("%s  <-- linea invalida\n", linea);
}
