#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int weight;
} TrieNode;

TrieNode *createNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (node)
    {
        node->isEndOfWord = 0;
        node->weight = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

typedef struct
{
    char word[100];
    int weight;
} WordWeightPair;

void insert(TrieNode *root, WordWeightPair wordWeightPair)
{
    const char *word = wordWeightPair.word;
    int weight = wordWeightPair.weight;
    TrieNode *current = root;
    while (*word)
    {
        int index = *word - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
        word++;
    }
    current->isEndOfWord = 1;
    current->weight = weight; // Update weight only for the end of word node
}

void autocompleteHelper(TrieNode *node, char *buffer, int index, const char *prefix, int prefixLength, WordWeightPair *suggestions, int *count)
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
            autocompleteHelper(node->children[i], buffer, index + 1, prefix, prefixLength, suggestions, count);
        }
    }
}

// Comparator function for sorting WordWeightPair array by weight
int compare(const void *a, const void *b)
{
    WordWeightPair *pairA = (WordWeightPair *)a;
    WordWeightPair *pairB = (WordWeightPair *)b;
    return pairB->weight - pairA->weight; // Sort in descending order
}

void autocomplete(TrieNode *root, const char *prefix)
{
    TrieNode *current = root;
    const char *originalPrefix = prefix;
    int prefixLength = strlen(prefix);
    while (*prefix)
    {
        int index = *prefix - 'a';
        if (!current->children[index])
        {
            return; // prefix not found
        }
        current = current->children[index];
        prefix++;
    }

    WordWeightPair suggestions[100]; // Adjust the size according to your needs
    int count = 0;
    char buffer[100];                                                                          // Adjust the size according to your needs
    autocompleteHelper(current, buffer, 0, originalPrefix, prefixLength, suggestions, &count); // Then call autocompleteHelper

    // Sort suggestions by weight
    qsort(suggestions, count, sizeof(WordWeightPair), compare);

    // Display suggestions
    for (int i = 0; i < count; i++)
    {
        printf("%s%s (f:%d)\n", originalPrefix, suggestions[i].word, suggestions[i].weight);
    }
}

void initializeDataset(TrieNode *root, WordWeightPair dataset[], int size)
{
    for (int i = 0; i < size; i++)
    {
        insert(root, dataset[i]);
    }
}

int main()
{
    TrieNode *root = createNode();
    WordWeightPair dataset[] = {{"apple", 3}, {"application", 5}, {"app", 2}, {"banana", 4}};
    int datasetSize = sizeof(dataset) / sizeof(dataset[0]);
    initializeDataset(root, dataset, datasetSize);

    int choice;
    char prefix[100];
    do
    {
        printf("Menu:\n");
        printf("1. Autocomplete\n");
        printf("2. Insert new word with weight\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int subChoice;
            do
            {
                printf("Enter prefix: ");
                scanf("%s", prefix);
                autocomplete(root, prefix);
                printf("Do you want to autocomplete again? (1 for yes, 0 to go back to main menu): ");
                scanf("%d", &subChoice);
            } while (subChoice != 0);
            break;
        }
        case 2:
        {
            WordWeightPair newWord;
            printf("Enter new word: ");
            scanf("%s", newWord.word);
            printf("Enter weight: ");
            scanf("%d", &newWord.weight);
            insert(root, newWord);
            break;
        }
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
