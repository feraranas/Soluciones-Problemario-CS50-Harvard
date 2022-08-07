// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "dictionary.h"
#include <stdlib.h>
#include  <stdio.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26; /* Por el número de letras en el abecedario */

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
// It will take a word and answer the question of: is this word in the dictionary or not?
bool check(const char *word)
{
    // TODO
    // We summon hash function so that we only check the index in which the word is at the linked list
    int indx = hash(word);

    // Iterate over the linked list O(n), where n is the length of linked list
    for (node *tmp = table[indx]; tmp != NULL; tmp = tmp -> next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Takes a word and runs a hash function returning some number that corresponds to that word

    char *a = "abcdefghijklmnopqrstuvwxyz";
    char *b = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /* hash function which maps the first letter of the word with the 26 alphabetics */
    int i;
    for (i = 0; i < N; i++)
    {
        if (*word == *(a + i) || *word == *(b + i))
        {
            return i;
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful, else false
// It's responsible for loading all of the words in the dictionary into a data structure: hash table
bool load(const char *dictionary)
{
    // TODO
    // Open up dictionary *file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    // Read from the dictionary into the hash table

    // Open a char array that can hold a word of length LENGTH
    char buffer[LENGTH + 1];

    while (fscanf(file, "%s", buffer) != EOF) { /* Read strings from dictionary file one at a time */
        node *n = malloc(sizeof(node)); /* Allocate memory for a new node (which contains a word, and a pointer to the next word) */
        if (n == NULL) /* Check if there's enough memory available */
        {
            return false; /* there was a memory error, i.e file couldn't be opened */
        }

        // Then put new information into that node. First copy the word into our node
        strcpy(n->word, buffer);

        // Then set the linked list to NULL
        n->next = NULL;

        // Call the hash function to determine which index into the hash table you should use when inserting this new node.
        int index = hash(n->word);

        // Insert word into hash table at the location given by hash function
        if (table[index] == NULL) { // If the table is empty on that index, that is, if it's the first node
            table[index] = n;
        } else { // If there's a linked list already on that index in the hash table
            n->next = table[index];
            table[index] = n;
        }
    }

    fclose(file);
    return true; /* Return true if you succesfully loaded all your data into your hash table */
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO: Iterating over every single one of the linked lists inside of the hash table, counting the number of nodes inside of the linked lists
    int i = 0;
    int count = 0;
    while (i < N)
    {
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            count++;
        }
        i++;
    }

    // Alternative solution: as I'm loading the hash table, keep track of the number of words added to the dictionary so far so that I can return that value in my size function
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
// Any of the memory you had to allocate, must now be free
bool unload(void)
{
    // TODO
    // Iterate over the hash table
    int i = 0;
    while (i < N)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        i++;
    }
    return true;
}
