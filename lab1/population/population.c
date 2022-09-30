#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt for start size
    int n;
    do
    {
        n = get_int("Please enter the start size: ");
    }
    while (n < 9);


    //Prompt for end size
    int m;
    do
    {
        m = get_int("Please enter the end size: ");
    }
    while (m < n);


    //Calculate number of years until we reach threshold
    int i;
    for (i = 0; n < m; i++)
    {
        n = n + (n / 3) - (n / 4);
    }

    //Print number of years
    printf("Years: %i\n", i);
}