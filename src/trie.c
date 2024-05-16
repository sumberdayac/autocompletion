#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/trie.h"

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

void insertWord(TrieNode *root, WordWeightPair wordWeightPair)
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
    current->weight = weight; // Update weight only for the end-of-word node
}
