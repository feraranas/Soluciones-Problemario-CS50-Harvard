#include <cs50.h>
#include <stdio.h>

int count_ccn(long a);
bool check_sum(int b, long c);

int main(void)
{
    long ccn;
    do
    {
        ccn = get_long("CCN: ");
    }
    while (ccn <= 0);

    int num = count_ccn(ccn);

    if (check_sum(num, ccn) == true)
    {
        // American Express uses 15-digit numbers & start with 34 or 37
        if ((ccn >= 34e13 && ccn < 35e13) || (ccn >= 37e13 && ccn < 38e13))
        {
            printf("AMEX\n");
        }

        // MasterCard uses 16-digit numbers & start with 51, 52, 53, 54, or 55
        else if (ccn >= 51e14 && ccn < 56e14)
        {
            printf("MASTERCARD\n");
        }

        // Visa uses 13- and 16-digit numbers & start with 4
        else if ((ccn >= 4e12 && ccn < 5e12) || (ccn >= 4e15 && ccn < 5e15))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

bool check_sum(int b, long c)
{
    // Guarda los impares, multiplicalos por dos y suma sus dígitos
    int cont = 0;
    for (long i = c, j = 10; i > 0; i /= 10, j *= 10)
    {
        int a = (((i % (j * 10)) - (i % j)) / j) * 2;
        int suma = (a / 10) + (a % 10);

        cont = cont + suma;
    }

    // Guarda los pares y sumamos los dígitos. Al final agregamos la suma anterior.
    int con = 0;
    for (long i = c, j = 1; i > 0; i = i / 10)
    {
        int a = ((i % (j * 10) - (i % j)) / j);

        con = con + a;
        j = j * 10;
    }

    if (((cont + con) % 10) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Contamos el número de dígitos de la credit card
int count_ccn(long a)
{
    // Guardamos el contador en count
    int count;
    for (count = 0; a > 0; count++)
    {
        // Vamos eliminando las unidades finales por cada paso
        a = a / 10;
    }
    return count;
}
/*
Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
Add the sum to the sum of the digits that weren’t multiplied by 2.
If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
*/