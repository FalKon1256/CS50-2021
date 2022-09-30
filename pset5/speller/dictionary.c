// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Seed value for hash function (Can be modified)
#define SEED 100

// Tracing dictionary word size
int wordSize = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Declaration of destroy function to free memory
void destroy(node *cursor);

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    
    // Converting input word to lowercase, word varible is static
    int len = strlen(word);
    char a[len + 1];
    for (int i = 0; i < len + 1; i++)
    {
        a[i] = tolower(word[i]);
    }
    
    // Apply hash function to recieve hash value of input word
    unsigned int index = hash(a);
    
    // Access linked list with the hash index & Search the input word
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (!strcasecmp(word, tmp->word))
        {
            return true;
        }
    }
    return false;
}


// Hashes word to a number

unsigned int hash(const char *word)
{
    // TODO
    
    // Apply hash function to recieve hash value
    /* One-byte-at-a-time hash based on Murmur's mix (MurmurHash)
       Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    unsigned int h = SEED;
    for (; *word; word++)
    {
        h ^= *word;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }

    // Recieve index for hash table
    h = h % N;

    return h;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    
    // Open dictionary input file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    
    char word[LENGTH + 1];
    
    // Load input file
    while (true)
    {
        // Read strings from input file one at a time & Determine end to stop
        if (fscanf(file, "%s", word) == EOF)
        {
            break;
        }
        else
        {
            // Create a new node for each word
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                fclose(file);
                return false;
            }
            
            // Copy each scan word to node
            strcpy(n->word, word);
            
            // Hash each word to get corresponding hash index
            unsigned int index = hash(word);
            
            // Insert node into the hash table's first position by corresponding hash index
            n->next = table[index];
            table[index] = n;
            
            // Trace word size
            wordSize++;
        }
    }
    fclose(file);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    
    // Recursion to free each link list
    for (int i = 0; i < N; i++)
    {
        destroy(table[i]);
        table[i] = NULL;
        
        // Unload failure
        if (table[i] != NULL)
        {
            return false;
        }
    }
    return true;
    

    
}


void destroy(node *cursor)
{
    if (cursor == NULL)
    {
        return;
    }
    
    destroy(cursor->next);
    free(cursor);
}