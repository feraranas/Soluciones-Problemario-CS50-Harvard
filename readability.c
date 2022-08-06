#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
int coleman_index(int letras, int palabras, int oraciones);

int main(void)
{
    char *s = get_string("Prompt: ");
    
    int a = count_letters(s);
    int b = count_words(s);
    int c = count_sentences(s);
    int n = coleman_index(a, b, c);
    
    if (n < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (n >= 1 && n <= 16)
    {
        printf("Grade %i\n", n);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int coleman_index(int letras, int palabras, int oraciones)
{
    float L = (letras * 100) / (float) palabras;
    float S = (oraciones * 100) / (float) palabras;
    return round(0.0588 * L - 0.296 * S - 15.8);
}

// Function for counting letters
int count_letters(char *parrafo)
{
    int count = 0;
    for (int i = 0, n = strlen(parrafo); i < n; i++)
    {
        if ((*(parrafo + i) >= 'A' && *(parrafo + i) <= 'Z') || (*(parrafo + i) >= 'a' && *(parrafo + i) <= 'z'))
        {
            count++;
        }
        else
        {
            
        }
    }
    return count;
}

// Function for counting words
int count_words(string parrafo)
{
    int count = 0;
    for (int i = 0, n = strlen(parrafo); i < n; i++)
    {
        if (*(parrafo + i) == ' ')
        {
            count++;
        }
    }
    return count + 1;
}

// Function for counting sentences
int count_sentences(string parrafo)
{
    int count = 0;
    for (int i = 0, n = strlen(parrafo); i < n; i++)
    {
        if (*(parrafo + i) == '!' || *(parrafo + i) == '.' || *(parrafo + i) == '?')
        {
            count++;
        }
    }
    return count;
}
