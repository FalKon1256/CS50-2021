#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, string argv[])
{

    // Determine if command-line argument is single
    if (argc == 2)
    {

        // Determine if key input is a number
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (isdigit(argv[1][i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

        // Convert command-line argument from "string" to "int" data form
        int key = atoi(argv[1]);

        // Prompt user to input plaintext
        string plaintext = get_string("plaintext: ");

        // Check each character of the plaintext and print the ciphertext out
        printf("ciphertext: ");

        for (int i = 0, n = strlen(plaintext); i < n; i++)
        {

            // If character is uppercase letter, rotate it and preserve uppercase, then print it out
            if (isupper(plaintext[i]))
            {
                char c = ((plaintext[i] - 'A' + key) % 26) + 'A';
                printf("%c", c);
            }

            // If character is lowercase letter, rotate it and preserve lowercase, then print it out
            else if (islower(plaintext[i]))
            {
                char c = ((plaintext[i] - 'a' + key) % 26) + 'a';
                printf("%c", c);
            }

            // Print the original character as is
            else
            {
                printf("%c", plaintext[i]);
            }
        }
        printf("\n");
    }

    // Prompt user with an error message
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}