#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/autocomplete.h"
#include "../include/trie.h"
#include "../include/fileProcess.h"
#include "fileProcess.c"

#define MAX_WORD_LENGTH 100
#define MAX_SUGGESTIONS 100

void autocompleteHelper(TrieNode *node, char *buffer, int index, WordWeightPair *suggestions, int *count)
{
    if (node->isEndOfWord)
    {
        buffer[index] = '\0';
        suggestions[*count].word = strdup(buffer);
        if (suggestions[*count].word == NULL)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        suggestions[*count].weight = node->weight;
        (*count)++;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i])
        {
            buffer[index] = 'a' + i;
            autocompleteHelper(node->children[i], buffer, index + 1, suggestions, count);
        }
    }
}

int compareWeights(const void *a, const void *b)
{
    WordWeightPair *pairA = (WordWeightPair *)a;
    WordWeightPair *pairB = (WordWeightPair *)b;
    return pairB->weight - pairA->weight; // Sort in descending order
}

void findSuggestions(TrieNode *root, const char *prefix, WordWeightPair *suggestions, int *count)
{
    TrieNode *current = root;
    const char *p = prefix;
    while (*p)
    {
        int index = *p - 'a';
        if (!current->children[index])
        {
            return; // Prefix not found
        }
        current = current->children[index];
        p++;
    }
    char buffer[MAX_WORD_LENGTH];
    strcpy(buffer, prefix);
    autocompleteHelper(current, buffer, strlen(prefix), suggestions, count);
}

void displaySuggestions(const char *prefix, WordWeightPair *suggestions, int count)
{
    qsort(suggestions, count, sizeof(WordWeightPair), compareWeights);
    for (int i = 0; i < count; i++)
    {
        printf("%s%s (f:%d)\n", prefix, suggestions[i].word + strlen(prefix), suggestions[i].weight);
        free(suggestions[i].word); // Free the allocated memory for word
    }
}

void handleAutocomplete(TrieNode *root)
{
    char *prefix = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    if (!prefix)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    printf("Enter prefix: ");
    scanf("%s", prefix);
    WordWeightPair suggestions[MAX_SUGGESTIONS];
    int count = 0;
    findSuggestions(root, prefix, suggestions, &count);
    displaySuggestions(prefix, suggestions, count);

    free(prefix);
}

void handleInsertNewWord(TrieNode *root)
{
    char *buffer = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    if (!buffer)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    int weight;

    printf("Enter new word: ");
    scanf("%s", buffer);

    printf("Enter weight: ");
    scanf("%d", &weight);

    TrieNode *current = root;
    const char *p = buffer;
    while (*p)
    {
        int index = *p - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNodeTrie();
        }
        current = current->children[index];
        p++;
    }

    if (current->isEndOfWord)
    {
        current->weight += weight; // Add to existing weight if the word is already there
    }
    else
    {
        current->isEndOfWord = 1;
        current->weight = weight; // Assign the new weight if the word is new
    }

    free(buffer);
}
