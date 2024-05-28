#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/trie.h"

TrieNode *createNodeTrie()
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

void insertWordTrie(TrieNode *root, WordWeightPair wordWeightPair)
{
    const char *word = wordWeightPair.word;
    int weight = wordWeightPair.weight;
    TrieNode *current = root;

    while (*word)
    {
        int index = *word - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNodeTrie();
        }
        current = current->children[index];
        word++;
    }
    current->isEndOfWord = 1;
    current->weight = weight; // Update weight only for the end-of-word node
}

void displayDictionary(TrieNode *root, const char *prefix)
{
    if (prefix == NULL || strlen(prefix) == 0)
    {
        char buffer[100];
        displayDictionaryHelper(root, buffer, 0);
    }
    else
    {
        TrieNode *current = root;
        char buffer[100];
        strcpy(buffer, prefix);
        int length = strlen(prefix);

        while (*prefix)
        {
            int index = *prefix - 'a';
            if (!current->children[index])
            {
                printf("No words found with the given prefix.\n");
                return;
            }
            current = current->children[index];
            prefix++;
        }

        displayDictionaryHelper(current, buffer, length);
    }
}

void displayDictionaryHelper(TrieNode *node, char *buffer, int index)
{
    if (node->isEndOfWord)
    {
        buffer[index] = '\0';
        printf("%s (Weight: %d)\n", buffer, node->weight);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i])
        {
            buffer[index] = 'a' + i;
            displayDictionaryHelper(node->children[i], buffer, index + 1);
        }
    }
}

void printTrie(TrieNode *root, char *prefix, bool *isLast, int level)
{
    if (root == NULL){
        return;
    }

    if (level > 0){
        for (int i = 0; i < level - 1; i++){
            if (isLast[i]){
                printf("    ");
            } else {
                printf("|   ");
            }
        }

        if (isLast[level - 1]){
            printf("`--");
        } else {
            printf("|--");
        }
    }

    printf("%s\n", prefix);

    int childCount = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (root->children[i]){
            childCount++;
        }
    }

    int currentChild = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (root->children[i]){
            isLast[level] = currentChild == childCount -1;
            char newPrefix[2] = {(char)('A' + i), '\0'};
            printTrie(root->children[i], newPrefix, isLast, level + 1);
            currentChild++;
        }
    }
}

void printTrieWithPrefix(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    while (*prefix) {
        int index = *prefix - 'a';
        if (!current->children[index]) {
            printf("Prefix not found in trie.\n");
            return;
        }
        current = current->children[index];
        prefix++;
    }

    bool isLast[100] = {0};
    // Print the root prefix
    char rootPrefix[2] = {(char)(prefix[-1] - 'a' + 'A'), '\0'}; // Convert to uppercase
    // printf("|--%s\n", rootPrefix);
    // Print the children of the current node
    printTrie(current, rootPrefix, isLast, 1);
}

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