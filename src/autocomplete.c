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
    gotoxy(0, 3);
    printf("Suggestions for %s:\n", prefix);
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
    COORD cursorPos = {0, 2};
    char pesan[100];
    if (!prefix || !buffer)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    prefix[0] = '\0';
    while (true)
    {
        gotoxy(cursorPos.X, cursorPos.Y);
        if (prefix[0] == '\0')
        {
            printHalfScreen("Enter prefix: ", true, false);
            scanf("%s", prefix);
        }
        else
        {
            sprintf(pesan, "Continue prefix: %s", prefix);
            printHalfScreen(pesan, true, false);
            scanf("%s", buffer);
            prefix = strcat(prefix, buffer);
        }

        WordWeightPair suggestions[MAX_SUGGESTIONS];
        int count = 0;
        findSuggestions(root, prefix, suggestions, &count);
        displaySuggestions(prefix, suggestions, count);

        if (count > 0)
        {
            gotoxy(cursorPos.X, cursorPos.Y);
            sprintf(pesan, "Select a suggestion (1-%d) or press 0 to continue typing: ", count);
            printHalfScreen(pesan, true, false);
            int choice;
            scanf("%d", &choice);
            if (choice > 0 && choice <= count)
            {
                strcpy(prefix, suggestions[choice - 1].word);
                sprintf(pesan, "Selected suggestion: %s", prefix);
                printHalfScreen(pesan, true, false);
            }
        }

        printHalfScreen("Do you want to continue? (y/n): ", true, false);
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

    gotoxy(0, 2);

    printHalfScreen("Enter new word: ", true, false);
    scanf("%s", buffer);

    printHalfScreen("Enter weight: ", true, false);
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

void deleteWordFromTrie(TrieNode *root, const char *word, bool *found)
{
    if (!root)
    {
        return;
    }

    TrieNode *stack[MAX_WORD_LENGTH];
    int indexStack[MAX_WORD_LENGTH];
    int top = 0;

    TrieNode *current = root;
    while (*word)
    {
        int index = *word - 'a';
        if (!current->children[index])
        {
            return; // Word not found
        }
        stack[top] = current;
        indexStack[top] = index;
        top++;
        current = current->children[index];
        word++;
    }

    if (current->isEndOfWord)
    {
        current->isEndOfWord = 0;
        current->weight = 0;
        *found = true;

        for (int i = top - 1; i >= 0; i--)
        {
            int index = indexStack[i];
            if (stack[i]->children[index]->isEndOfWord)
            {
                break;
            }
            bool hasChildren = false;
            for (int j = 0; j < ALPHABET_SIZE; j++)
            {
                if (stack[i]->children[index]->children[j])
                {
                    hasChildren = true;
                    break;
                }
            }
            if (!hasChildren)
            {
                free(stack[i]->children[index]);
                stack[i]->children[index] = NULL;
            }
            else
            {
                break;
            }
        }
    }
}

void deleteWordFromDictionary(vocabNode **head, const char *word, bool *found)
{
    vocabNode *current = *head;
    vocabNode *prev = NULL;

    while (current != NULL && strcmp(current->word, word) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        return; // Word not found
    }

    if (prev == NULL)
    {
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    free(current->word);
    free(current);
    *found = true;
}

void handleDeleteWord(TrieNode *trieRoot, vocabNode **head)
{
    bool foundInTrie = false;
    bool foundInDict = false;
    char word[MAX_WORD_LENGTH];

    gotoxy(0, 2);
    printHalfScreen("Enter word to delete: ", true, false);
    scanf("%s", word);

    deleteWordFromTrie(trieRoot, word, &foundInTrie);
    deleteWordFromDictionary(head, word, &foundInDict);

    if (foundInTrie && foundInDict)
    {
        printf("Word '%s' has been successfully deleted from both trie and dictionary.\n", word);
    }
    else if (foundInTrie)
    {
        printf("Word '%s' has been successfully deleted from trie.\n", word);
    }
    else if (foundInDict)
    {
        printf("Word '%s' has been successfully deleted from dictionary.\n", word);
    }
    else
    {
        printf("Word '%s' not found in trie or dictionary.\n", word);
    }

    saveData(*head, "words.txt");
}