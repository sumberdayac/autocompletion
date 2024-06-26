#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include "trie.h"

void autocompleteHelper(TrieNode *node, char *buffer, int index, WordWeightPair *suggestions, int *count);
void findSuggestions(TrieNode *root, const char *prefix, WordWeightPair *suggestions, int *count);
void displaySuggestions(const char *prefix, WordWeightPair *suggestions, int count);
void handleAutocomplete(TrieNode *root);
void handleInsertNewWord(TrieNode *root);
void displayMenu();
void deleteWordFromTrie(TrieNode *root, const char *word, bool *found);
void deleteWordFromDictionary(vocabNode **head, const char *word, bool *found);
void handleDeleteWord(TrieNode *trieRoot, vocabNode **head);

#endif // AUTOCOMPLETE_H
