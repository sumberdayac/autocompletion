#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mainProcess.h"
#include "../include/trie.h"
#include "../include/autocomplete.h"
#include "../include/fileProcess.h"
#include "autocomplete.c"

void displayMenu()
{
    printf("Menu:\n");
    printf("1. Autocomplete\n");
    printf("2. Display Dictionary\n");
    printf("3. Insert new word with weight\n");
    printf("4. Load reference file\n");
    printf("5. Print Trie\n");
    printf("6. Delete word\n");
    printf("0. Exit\n");
}

void mainProcess()
{
    vocabNode *head = NULL;
    const char *initialDataFile = "words.txt";
    loadInitialData(&head, initialDataFile);

    TrieNode *root = createNodeTrie();
    initializeTrieFromList(root, head);

    int choice;
    do
    {
        system("cls");
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:
            handleAutocomplete(root);
            getch();
            break;
        case 2:
            displayDictionary(root);
            getch();
            break;
        case 3:
            handleInsertNewWord(root);
            getch();
            break;
        case 4:
            updateDictionaryFromFile(&head);
            getch();
            break;
        case 5:
            bool isLast[MAX_WORD_LENGTH] = {0};
            char prefix[2] = " ";
            printTrie(root, prefix, isLast, 0);
            getch();
            break;
        case 6:
            handleDeleteWord(root, &head);
            getch();
            break;
        case 0:
            printf("Exiting...\n");
            getch();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 0);

    saveData(head, initialDataFile);
    freeList(head);
}