#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define string char*

typedef struct vocabNode {
    string word;
    int weight;
    struct vocabNode *next;
} vocabNode;

vocabNode* createNode(const char *word, int weight) {
    vocabNode *newNode = (vocabNode *)malloc(sizeof(vocabNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    newNode->word = strdup(word);
    if (newNode->word == NULL) {
        fprintf(stderr, "Memory allocation for word failed.\n");
        free(newNode);
        exit(1);
    }
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

vocabNode* findWord(vocabNode *head, const char *word) {
    vocabNode *current = head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void addWord(vocabNode **head, const char *word) {
    vocabNode *foundNode = findWord(*head, word);
    if (foundNode) {
        foundNode->weight += 1;
    } else {
        vocabNode *newNode = createNode(word, 1);
        newNode->next = *head;
        *head = newNode;
    }
}

void freeList(vocabNode *head) {
    vocabNode *current = head;
    vocabNode *next;
    while (current != NULL) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}

void processText(vocabNode **head, const char *text) {
    char buffer[100];
    int i = 0;
    while (*text) {
        if (isalpha(*text)) {
            buffer[i++] = tolower(*text);
        } else if (i > 0) {
            buffer[i] = '\0';
            addWord(head, buffer);
            i = 0;
        }
        text++;
    }
    if (i > 0) {
        buffer[i] = '\0';
        addWord(head, buffer);
    }
}

char* readFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc((length + 1) * sizeof(char));
    if (content == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(file);
        exit(1);
    }

    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

void loadInitialData(vocabNode **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *equalSign = strchr(line, '=');
        if (equalSign) {
            *equalSign = '\0';
            char *word = line;
            int weight = atoi(equalSign + 1);
            addWord(head, word);
            findWord(*head, word)->weight = weight; // Set the correct weight
        }
    }

    fclose(file);
}

void saveData(vocabNode *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    vocabNode *current = head;
    while (current != NULL) {
        fprintf(file, "%s=%d\n", current->word, current->weight);
        current = current->next;
    }

    fclose(file);
}

int main() {
    vocabNode *head = NULL;
    const char *initialDataFile = "words.txt";
    
    printf("Masukkan nama file referensi dengan ekstensi txt: ");
    
    char filename[256];
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename.\n");
        return 1;
    }
    
    // Append ".txt" to the filename
    char newTextFile[260]; // Ensure there's enough space
    snprintf(newTextFile, sizeof(newTextFile), "%s.txt", filename);
    
    // Load initial data from words.txt
    loadInitialData(&head, initialDataFile);
    
    // Read the new text content from the reference file
    char *newText = readFileContent(newTextFile);
    if (newText == NULL) {
        fprintf(stderr, "Error reading reference file.\n");
        freeList(head);
        return 1;
    }
    
    // Process the new text to update the linked list
    processText(&head, newText);
    
    // Save the updated linked list back to words.txt
    saveData(head, initialDataFile);
    
    // Free the allocated memory for the linked list and the new text
    freeList(head);
    free(newText);
    
    return 0;
}

