#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26

#include <stdbool.h>
#include "fileProcess.h"

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int weight;
} TrieNode;

typedef struct {
    char *word;
    int weight;
} WordWeightPair;

TrieNode *createNodeTrie();
void insertWordTrie(TrieNode *root, WordWeightPair wordWeightPair);
void displayDictionary(TrieNode *root);
void displayDictionaryHelper(TrieNode *node, char *buffer, int index);
void printTrie(TrieNode *root, char *prefix, bool *isLast, int level);
void initializeTrieFromList(TrieNode *root, vocabNode *head);

#endif // TRIE_H
