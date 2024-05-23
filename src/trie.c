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

void displayDictionary(TrieNode *root)
{
    char buffer[100];
    displayDictionaryHelper(root, buffer, 0);
}

void displayDictionaryHelper(TrieNode *node, char *buffer, int index)
{
    if (node->isEndOfWord)
    {
        buffer[index] = '\0';
        printf("%s (f:%d)\n", buffer, node->weight);
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