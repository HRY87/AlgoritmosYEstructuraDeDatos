#include "lotePrueba.h"

// Simula como se generaron los archivos del enunciado
int generarArchTxtCortado(const char *nombArch, const char *texto)
{
    FILE *arch;
    const char *fin;
    size_t cant;

    if(!(arch = fopen(nombArch, "wt")))
        return ERROR_ARCH;

    while(*texto)
    {
        fin = texto + strcspn(texto, "\n");     // Fin de la linea original

        do                                      // do-while: tambien escribe lineas vacias
        {
            cant = MINIMO((size_t)MAXLINEA, (size_t)(fin - texto));
            fwrite(texto, 1, cant, arch);
            fputc('\n', arch);
            texto += cant;
        }
        while(texto < fin);

        texto = *fin ? fin + 1 : fin;           // Saltea el '\n' original
    }

    fclose(arch);
    return TODO_BIEN;
}

/*
 * Casos cubiertos (con MAXLINEA = 10):
 *  - Palabra cortada en 2 lineas (Alber|to, Spinett|a, due|rme, esper|a, A|na, 1|23)
 *  - Palabra cortada en 3 lineas (supercalifragilistico)
 *  - Linea corta que termina en letra y la siguiente empieza en letra (no se unen)
 *  - Palabras repetidas dentro de un mismo arbol (Ana)
 *  - Delimitadores variados (',', '-', ';') y palabras con numeros (abc123, x9)
 *  - Linea vacia en B
 *  - Palabras en ambos arboles, solo en A y solo en B
 * Ninguna linea original mide exactamente MAXLINEA (ambiguo, ver sugerencias).
 *
 * Salida esperada de A - B (orden ASCII):
 *  Alberto Palabra Sola abc123 cuarto dos duerme en espera fin no su x9 y
 */
int generarLote(const char *nombA, const char *nombB)
{
    const char *textoA =
        "Luis Alberto Spinetta\n"
        "Ana no duerme, espera el dia\n"
        "Sola en su cuarto, Ana quiere jugar,\n"
        "Palabra supercalifragilistico y fin\n"
        "uno-dos;tres abc123 x9\n";

    const char *textoB =
        "Luis Spinetta\n"
        "Ana quiere jugar\n"
        "el dia\n"
        "\n"
        "uno tres 123\n"
        "supercalifragilistico\n";

    if(generarArchTxtCortado(nombA, textoA) != TODO_BIEN)
        return ERROR_ARCH;

    return generarArchTxtCortado(nombB, textoB);
}
