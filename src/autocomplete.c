#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/autocomplete.h"

#define MAX_WORD_LENGTH 100
#define MAX_SUGGESTIONS 100

void autocompleteHelper(TrieNode *node, char *buffer, int index, WordWeightPair *suggestions, int *count)
{
    if (node->isEndOfWord)
    {
        buffer[index] = '\0';
        strcpy(suggestions[*count].word, buffer);
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
    autocompleteHelper(current, buffer, 0, suggestions, count);
}

void displaySuggestions(const char *prefix, WordWeightPair *suggestions, int count)
{
    qsort(suggestions, count, sizeof(WordWeightPair), compareWeights);
    for (int i = 0; i < count; i++)
    {
        printf("%s%s (f:%d)\n", prefix, suggestions[i].word, suggestions[i].weight);
    }
}

void handleAutocomplete(TrieNode *root)
{
    char prefix[MAX_WORD_LENGTH];
    printf("Enter prefix: ");
    scanf("%s", prefix);
    WordWeightPair suggestions[MAX_SUGGESTIONS];
    int count = 0;
    findSuggestions(root, prefix, suggestions, &count);
    displaySuggestions(prefix, suggestions, count);
}

void handleInsertNewWord(TrieNode *root)
{
    WordWeightPair newWord;
    printf("Enter new word: ");
    scanf("%s", newWord.word);
    printf("Enter weight: ");
    scanf("%d", &newWord.weight);
    insertWord(root, newWord);
}
