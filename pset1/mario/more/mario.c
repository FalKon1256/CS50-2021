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
 
 
    //Print out pyramid
    for (int i = 0; i < n; i++)
    {
        //Print out left-side space
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }

        //Print out left-side # brick
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        
        //Print out spaces at middle
        for (int j = 0; j < 2; j++)
        {
            printf(" ");
        }
        
        //Print out right-side # brick
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}