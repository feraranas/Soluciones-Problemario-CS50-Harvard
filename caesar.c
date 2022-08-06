#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int convert_s_to_i(char *s);

int main(int argc, char *argv[])
{
    // In case the user enters more than 1 command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // In case the user doesn't enter a decimal digit
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (argv[1][i] < '0' || argv[1][i] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // Las lineas anteriores me están diciendo que el usuario inputo un string
    // Pero no lo leo como un int, sino como valores ASCII
    // Donde 0 == 48 y 9 == 57, entonces lo que realmente estoy comparando es:
    // argv[1][i] < 48 y argv[1][i] > 57 para cada "string"
    // Por ejemplo: el usuario inputo 123. Esto se lee: {'49' '50' '51'}
    // Es decir, un array de esos ASCII digits o más bien char's

    // Convertimos la key a un entero
    int key = convert_s_to_i(argv[1]);

    // Prompt for the plaintext
    printf("plaintext: ");
    char *plaintext = get_string("");
    printf("ciphertext: ");

    // Allocating space the same size of plaintext
    char *ciphertext = malloc(strlen(plaintext));
    // OJO: Un char = 1 bytes
    //      Un int  = 4 bytes

    // Enciphering the plaintext
    // En este loop ya puedo imprimir el cipher despues de "ciphertext"
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // Uppercase remain Uppercase
        if (*(plaintext + i) >= 'A' && *(plaintext + i) <= 'Z')
        {
            *(ciphertext + i) = (((*(plaintext + i) - 'A') + key) % 26) + 'A';
            printf("%c", *(ciphertext + i));
        }
        // Lowercase remain Lowercase
        else if (*(plaintext + i) >= 'a' && *(plaintext + i) <= 'z')
        {
            *(ciphertext + i) = (((*(plaintext + i) - 'a') + key) % 26) + 'a';
            printf("%c", *(ciphertext + i));
        }
        else // Meaning that any char that doesn't represent a letter remains the same
        {
            printf("%c", *(plaintext + i));
        }
    }

    // Fórmula ci = (pi + k) % 26

    printf("\n");
    return 0;
}

int convert_s_to_i(char *s)
{
    // Definimos el tamaño de elementos (n) en el string (key)
    int n = strlen(s);

    // Abrimos un array de n elementos
    int a[n];

    // Separamos cada elemento y lo insertamos al array
    for (int i = 0; i < n; i++)
    {
        a[i] = *(s + i) - '0';
    }

    // Inicializamos el entero a cero
    int entero = 0;

    // Asignamos a cada elemento ceros (000,000) y luego los sumamos
    // Empezamos en el último dígito y bajamos hasta el primero
    for (int i = n - 1, j = 1; i >= 0; i--)
    {
        /*  Si el key es de 1 dígito entonces multiplicamos por unidades (1)
            Si el key es de 2 dígitos entonces multiplicamos por decenas (10)
            Si el key es de 3 digitos entonces multiplicamos por centenas (100)
            Si el key es de 4 digitos entonces multiplicamos por millares (1000)
        .
        .
        .
        */
        a[i] = a[i] * j;
        j = j * 10;

        // Sumamos las unidad, decenas, centenas, millares, ...
        // int b = a[0] + a[1] + a[2] + a[3] + a[4];
        entero += a[i];
    }

    return entero;
}