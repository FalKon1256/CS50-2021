#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO

    // Sort vote result
    candidate sorted_result[candidate_count];
    candidate copy[candidate_count];
    candidate tmp[1];
    for (int i = 0; i < candidate_count; i++)
    {
        copy[i] = candidates[i];
    }
    
    for (int i = 0; i < candidate_count; i++)
    {
        sorted_result[i] = copy[i];
        for (int j = i; j < candidate_count; j++)
        {
            if (sorted_result[i].votes < copy[j].votes)
            {
                tmp[0] = copy[j];
                copy[j] = sorted_result[i];
                sorted_result[i] = tmp[0];
            }
        }
    }

    // Find and print the winner(s)
    printf("%s\n", sorted_result[0].name);
    for (int i = 1; i < candidate_count; i++)
    {
        if (sorted_result[i - 1].votes > sorted_result[i].votes)
        {
            return;
        }
        else if (sorted_result[i - 1].votes == sorted_result[i].votes)
        {
            printf("%s\n", sorted_result[i].name);
        }
    }
}