#include <cs50.h>
#include <stdio.h>
#include <math.h>

int number_quarters(int change, int quarters);
int number_dimes(int remain_one, int dimes);
int number_nickels(int remain_two, int nickels);
int number_pennies(int remain_three, int pennies);


int main(void)
{
    // Input the change owed to the customer
    float n;
    do
    {
        n = get_float("Owed quantitiy: ");
    }
    while (n < 0);
    
    // Declares the coins available for change
    int quarters = 25;
    int dimes = 10;
    int nickels = 5;
    int pennies = 1;
    
    // Converts the change owed from float to an integer
    int change = round(n * 100);

    // Counts the number of quarters handed to the customer
    int q = number_quarters(change, quarters);
    
    // Remaining change
    int remain_one = change - (q * quarters);
    
    // Counts the number of dimes handed to the customer
    int d = number_dimes(remain_one, dimes);
    
    // Remaining change
    int remain_two = remain_one - (d * dimes);
    
    // Counts the number of nickels handed to the customer
    int ni = number_nickels(remain_two, nickels);
    
    // Remaining change
    int remain_three = remain_two - (ni * nickels);
    
    // Counts the number of pennies handed to the customer
    int p = number_pennies(remain_three, pennies);

    printf("%i\n", q + d + ni + p);
}

int number_quarters(int change, int quarters)
{
    // Sets counter
    int counter = 0;
    
    // Counts number of quarters
    for (int i = 0; change >= quarters; i++)
    {
        change -= quarters;
        counter ++;
    }
    
    // Save the counter
    return counter;
}

int number_dimes(int remain_one, int dimes)
{
    // Sets counter
    int counter = 0;
    
    // Counts number of dimes
    for (int i = 0; remain_one >= dimes; i++)
    {
        remain_one -= dimes;
        counter ++;
    }
    
    // Save the counter
    return counter;
}

int number_nickels(int remain_two, int nickels)
{
    // Sets counter
    int counter = 0;
    
    // Counts number of dimes
    for (int i = 0; remain_two >= nickels; i++)
    {
        remain_two -= nickels;
        counter ++;
    }
    
    // Save the counter
    return counter;
}

int number_pennies(int remain_three, int pennies)
{
    // Sets counter
    int counter = 0;
    
    // Counts number of dimes
    for (int i = 0; remain_three >= pennies; i++)
    {
        remain_three -= pennies;
        counter ++;
    }
    
    // Save the counter
    return counter;
}