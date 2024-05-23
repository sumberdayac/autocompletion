#ifndef VOCABNODE_H
#define VOCABNODE_H

#define string char*

typedef struct vocabNode {
    string word;
    int weight;
    struct vocabNode *next;
} vocabNode;

vocabNode* createNode(const char *word, int weight);
vocabNode* findWord(vocabNode *head, const char *word);
void addWord(vocabNode **head, const char *word);
void freeList(vocabNode *head);
void processText(vocabNode **head, const char *text);
char* readFileContent(const char *filename);
void loadInitialData(vocabNode **head, const char *filename);
void saveData(vocabNode *head, const char *filename);
void updateLinkedListFromFile(vocabNode **head);

#endif // VOCABNODE_H