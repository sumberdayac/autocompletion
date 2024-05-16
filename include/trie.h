#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int weight;
} TrieNode;

typedef struct
{
    char word[100];
    int weight;
} WordWeightPair;

TrieNode *createNode();
void insertWord(TrieNode *root, WordWeightPair wordWeightPair);
void displayDictionary(TrieNode *root);
void displayDictionaryHelper(TrieNode *node, char *buffer, int index);

#endif // TRIE_H
