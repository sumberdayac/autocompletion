#include <stdio.h>
#include "../include/main.h"
#include "../include/trie.h"
#include "../include/autocomplete.h"
#include "autocomplete.c"
#include "trie.c"

void initializeTrie(TrieNode *root, WordWeightPair dataset[], int size)
{
    for (int i = 0; i < size; i++)
    {
        insertWord(root, dataset[i]);
    }
}

void displayMenu()
{
    printf("Menu:\n");
    printf("1. Autocomplete\n");
    printf("2. Insert new word with weight\n");
    printf("0. Exit\n");
}

int main()
{
    TrieNode *root = createNode();
    WordWeightPair dataset[] = {{"apple", 3}, {"application", 5}, {"app", 2}, {"banana", 4}};
    int datasetSize = sizeof(dataset) / sizeof(dataset[0]);
    initializeTrie(root, dataset, datasetSize);

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
            handleInsertNewWord(root);
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 0);

    return 0;
}
