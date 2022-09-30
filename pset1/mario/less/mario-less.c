#include <cs50.h>
#include <stdio.h>


int main(void)
{
    //Prompt user for positive integer
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
 
 
    //Print out right-side pyramid
    for (int i = 0; i < n; i++)
    {
        //Print out space
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }

        //Print out # brick
        for (int w = 0; w < i + 1; w++)
        {
            printf("#");
        }
        printf("\n");
    }
}