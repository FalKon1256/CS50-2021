#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    //Prompt user to input positive integer
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    
    //Round dollars to cents
    int cents = round(dollars * 100);



    
    //Quarters change
    int a = 0, b = 0, c = 0, d = 0;
    
    if (cents >= 25)
    {
        a = cents / 25;
        cents %= 25;
    }

    //Dimes change
    if (cents < 25 && cents >= 10)
    {
        b = cents / 10;
        cents %= 10;
    }

    //Nickels change
    if (cents < 10 && cents >= 5)
    {
        c = cents / 5;
        cents %= 5;
    }

    //Pennies change    
    if (cents < 5 && cents >= 1)
    {
        d = cents / 1;
        cents %= 1;
    }
    
    //Sum and print total of coins
    int coins;
    coins = a + b + c + d;
    printf("%i\n", coins);
}


