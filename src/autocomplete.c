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
    printf("\nSuggestions for %s:\n", prefix);
    for (int i = 0; i < count; i++)
    {
        printf("%d. %s%s\n", i+1, prefix, suggestions[i].word + strlen(prefix));
        free(suggestions[i].word); // Free the allocated memory for word
    }
}

void handleAutocomplete(TrieNode *root)
{
    char *prefix = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    char *buffer = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    if (!prefix || !buffer)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    prefix[0] = '\0';
    while (true)
    {
        if (prefix[0] == '\0')
        {
            printf("Enter prefix: ");
            scanf("%s", prefix);
        }
        else
        {
            printf("continue prefix: %s", prefix);
            scanf("%s", buffer);
            prefix = strcat(prefix, buffer);
        }

        WordWeightPair suggestions[MAX_SUGGESTIONS];
        int count = 0;
        findSuggestions(root, prefix, suggestions, &count);
        displaySuggestions(prefix, suggestions, count);

        if (count > 0)
        {
            printf("Select a suggestion (1-%d) or press 0 to continue typing: ", count);
            int choice;
            scanf("%d", &choice);
            if (choice > 0 && choice <= count)
            {
                strcpy(prefix, suggestions[choice - 1].word);
                printf("Selected suggestion: %s\n", prefix);
            }
        }

        printf("\nDo you want to continue? (y/n): ");
        char continueChoice;
        scanf(" %c", &continueChoice);
        if (continueChoice == 'n')
        {
            break;
        }
    }

    free(prefix);
    free(buffer);
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

    vocabNode *head = NULL;
    loadInitialData(&head, "words.txt");

    vocabNode *foundNode = findWord(head, buffer); // Corrected: pass head instead of &head
    if (foundNode)
    {
        foundNode->weight += weight;
    }
    else
    {
        vocabNode *newNode = createNodeDictionary(buffer, weight);
        newNode->next = head; // Corrected: head itself, not &head
        head = newNode;
    }

    saveData(head, "words.txt");

    free(buffer);
}