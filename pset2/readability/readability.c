#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int main(void)
{
    // Prompting the user for a string of text
    string text = get_string("Test: ");
    int n = strlen(text);

    // Calculate letters
    int letter = 0;

    for (int i = 0; i < n; i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            letter += 1;
        }
    }

    // Calculate words
    int word = 1;

    for (int i = 0; i < n; i++)
    {
        if (isspace(text[i]))
        {
            word += 1;
        }
    }

    // Calculate sentences
    int sentence = 0;

    for (int i = 0; i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence += 1;
        }
    }

    // Coleman-Liau formula
    float L = (letter / (float)word) * 100;
    float S = (sentence / (float)word) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);


    // Determine grades
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}