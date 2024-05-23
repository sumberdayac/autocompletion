#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"
#include "../include/trie.h"
#include "../include/autocomplete.h"
#include "../include/fileProcess.h"
#include "autocomplete.c"

void initializeTrieFromList(TrieNode *root, vocabNode *head)
{
    vocabNode *current = head;
    while (current != NULL)
    {
        WordWeightPair pair;
        pair.word = strdup(current->word);
        if (pair.word == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        pair.weight = current->weight;
        insertWordTrie(root, pair);
        free(pair.word);
        current = current->next;
    }
}

void displayMenu()
{
    printf("Menu:\n");
    printf("1. Autocomplete\n");
    printf("2. Display Dictionary\n");
    printf("3. Insert new word with weight\n");
    printf("4. Print Trie\n");
    printf("0. Exit\n");
}

int main()
{
    vocabNode *head = NULL;
    const char *initialDataFile = "words.txt";
    loadInitialData(&head, initialDataFile);

    TrieNode *root = createNodeTrie();
    initializeTrieFromList(root, head);

    int choice;
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            handleAutocomplete(root);
            break;
        case 2:
            displayDictionary(root);
            break;
        case 3:
            handleInsertNewWord(root);
            break;
        case 4:
            bool isLast[MAX_WORD_LENGTH] = {0};
            char prefix[2] = " ";
            printTrie(root, prefix, isLast, 0);
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 0);

    saveData(head, initialDataFile);
    freeList(head);

    return 0;
}
