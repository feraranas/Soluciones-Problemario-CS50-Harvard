#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool check(char *s);

int main(int argc, char *argv[])
{
    // In case the user enters more than 1 command-line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Count the number of characters in the command-line argument key
    int n = strlen(argv[1]);

    // In case the user doesn't enter a 26 character long key
    if (n != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check the key for any non-alphabetical character
    for (int i = 0; i < n; i++)
    {
        if ((argv[1][i] >= 65 && argv[1][i] <= 90) || (argv[1][i] >= 97 && argv[1][i] <= 122))
        {

        }
        else
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    // Checamos caracteres del abecedario repetidos
    if (check(argv[1]) == 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Prompt for the plaintext
    printf("plaintext: ");
    char *plaintext = get_string("");
    printf("ciphertext: ");

    // En este loop ya imprimo el cipher despues de "ciphertext"
    for (int i = 0, a = strlen(plaintext); i < a; i++)
    {
        // Si el texto que ingreso el usuario es mayúscula
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            // Si la llave está en mayúscula
            if (argv[1][plaintext[i] - 'A'] >= 'A' && argv[1][plaintext[i] - 'A'] <= 'Z')
            {
                // Imprime el cipher sin modificar
                printf("%c", argv[1][plaintext[i] - 'A']);
            }
            // Si la llave está en minúscula
            else if (argv[1][plaintext[i] - 'A'] >= 'a' && argv[1][plaintext[i] - 'A'] <= 'z')
            {
                // Imprime el cipher modificado
                printf("%c", argv[1][plaintext[i] - 'A'] - 32);
            }
        }
        // Si el texto que ingreso el usuario es minúscula
        else if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            // Si la llave esta en minúscula
            if (argv[1][plaintext[i] - 'a'] >= 'a' && argv[1][plaintext[i] - 'a'] <= 'z')
            {
                // Imprime el cipher sin modificarlo
                printf("%c", argv[1][plaintext[i] - 'a']);
            }
            // Si la llave esta en mayúscula
            else if (argv[1][plaintext[i] - 'a'] >= 65 && argv[1][plaintext[i] - 'a'] <= 90)
            {
                // Imprime el cipher modificado
                printf("%c", argv[1][plaintext[i] - 'a'] + 32);
            }
        }
        else // Meaning that any char that doesn't represent a letter remains the same
        {
            printf("%c", plaintext[i]);
        }
    }

    printf("\n");
    return 0;
}

bool check(char *s)
{
    /* Checking that there are no repeated letters of the alphabet in the key
    PSEUDOCODE:
    Iterate the key from i'th = 0 to i'th = n - 1
        Remember the i'th character
            If no i'th character doesn't repeat from i'th + 1 to n - 1
                return true
            Else
                return false

    O(n^2) "Order of 'n^2'"
    */

    // This for loop checks every character in the key once
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // This for loop checks every character in the key once
        for (int j = i + 1; j < n; j++)
        {
            if (*(s + i) == *(s + j))
            {
                return 1;
            }
        }
    }
    return 0;
}