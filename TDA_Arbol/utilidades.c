#include "utilidades.h"

/// ==================================================================
/// Funciones Int
/// ==================================================================

void mostrarInt(void *info, void *param)
{
    printf("%d\n",*(int*)info);
}

int cmpInt(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

/// ==================================================================
/// Funciones de Estructuras de Prueba
/// ==================================================================

int generarLoteAlu(char *nombArch)
{
    tAlu alumnos[] = {
        {1020, "Lourdes Rivas",     "Medicina",      9.4f, 'A'},
        {1001, "Agustina Losada",   "Ing. Sistemas", 8.1f, 'A'},
        {1015, "Federico Torres",   "Economía",      7.9f, 'A'},
        {1007, "Luciano Álvarez",   "Ingeniería",    6.2f, 'B'},
        {1004, "Carla Rodríguez",   "Medicina",      9.0f, 'A'},
        {1013, "Nahuel López",      "Derecho",       5.2f, 'B'},
        {1003, "Diego Torrez",      "Contador",      6.9f, 'B'},
        {1017, "Gonzalo Medina",    "Ing. Sistemas", 6.0f, 'B'},
        {1010, "Camila Sánchez",    "Contador",      4.9f, 'B'},
        {1005, "Pedro Ramírez",     "Derecho",       5.5f, 'B'},
        {1014, "Celeste Moya",      "Ing. Quimica",  7.1f, 'A'},
        {1002, "Sabrina Algo",      "Ing. Quimica",  7.5f, 'A'},
        {1011, "Bruno Herrera",     "Medicina",      8.0f, 'A'},
        {1019, "Tomás Vera",        "Contador",      5.8f, 'B'},
        {1006, "Ingrid Monsalve",   "Medicina",      8.7f, 'A'},
        {1022, "Antonella Acosta",  "Psicología",    9.1f, 'A'},
        {1023, "Rodrigo Espíndola", "Abogacía",      6.3f, 'B'},
        {1021, "Valeria Silva",     "Ing. Sistemas", 7.7f, 'A'},
        {1018, "Julieta Suárez",    "Abogacía",      8.5f, 'A'},
        {1008, "Florencia Gómez",   "Abogacía",      9.3f, 'A'},
        {1024, "Carlos Funes",      "Biología",      6.8f, 'B'},
        {1012, "Valentina Díaz",    "Psicología",    6.4f, 'A'},
        {1016, "Melina Barreto",    "Biología",      9.1f, 'A'},
        {1009, "Martín Villalba",   "Ing. Sistemas", 7.8f, 'A'},
        {1025, "Lucía Roldán",      "Economía",      8.2f, 'A'},
        {1027, "Ignacio Berón",     "Contador",      6.5f, 'B'},
        {1026, "Andrea Quiroga",    "Medicina",      9.0f, 'A'},
        {1028, "Mauro Ferreyra",    "Ing. Quimica",  5.4f, 'B'},
        {1029, "Gabriela Soto",     "Psicología",    7.2f, 'A'},
        {1030, "Romina Pereyra",    "Ingeniería",    8.9f, 'A'}
    };

    return crearArchBin(nombArch, alumnos, sizeof(alumnos) / sizeof(*alumnos), sizeof(*alumnos));
}

void mostrarAluCabe()
{
    printf("%8s  %-30s  %-20s  %8s  %6s\n", "LEGAJO", "NOMBRE", "CARRERA", "PROM", "EST");
}
void mostrarAlu(void *info, void *param)
{
    const tAlu *a = (const tAlu *)info;
    printf("%8d  %-30s  %-20s  %8.2f  %6c\n", a->legajo, a->nombre, a->carrera, a->promedio, a->estado);
}

int cmpAlu(const void *a, const void *b)
{
    const tAlu *al1 = (const tAlu *)a;
    const tAlu *al2 = (const tAlu *)b;

    return al1->legajo - al2->legajo;
}

void extraerClaveAlu(void *info, void *destClave)
{
    const tAlu *a = (const tAlu *)info;
    *(int*)destClave = a->legajo;
}

int filtroEstAlu(void *info, void *param)
{
    tAlu *alu = info;
    return alu->estado == *(char*)param ? ELIM : 1;
}

void bajaAlu(void *info, void *param)
{
    tAlu *alu = info;
    alu->estado = 'B';
}