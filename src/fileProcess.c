#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/fileProcess.h"

#define string char*

typedef struct vocabNode {
    string word;
    int weight;
    struct vocabNode *next;
} vocabNode;

vocabNode* createNode(const char *word, int weight) {
    vocabNode *newNode = (vocabNode *)malloc(sizeof(vocabNode));
    if (newNode == NULL) {
        fprintf(stderr, "Alokasi memori gagal.\n");
        exit(1);
    }
    newNode->word = strdup(word);
    if (newNode->word == NULL) {
        fprintf(stderr, "Alokasi memori gagal.\n");
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
        fprintf(stderr, "File gagal dibuka.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc((length + 1) * sizeof(char));
    if (content == NULL) {
        fprintf(stderr, "Alokasi memori gagal.\n");
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
        fprintf(stderr, "File gagal dibuka.\n");
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
        fprintf(stderr, "File gagal dibuka.\n");
        exit(1);
    }

    vocabNode *current = head;
    while (current != NULL) {
        fprintf(file, "%s=%d\n", current->word, current->weight);
        current = current->next;
    }

    fclose(file);
}

void updateDictionaryFromFile(vocabNode **head) {
    printf("Masukkan nama file referensi dengan ekstensi txt: ");
    
    char filename[256];
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "File tidak ditemukan!\n");
        return;
    }
    
    // Append ".txt" to the filename
    char newTextFile[260]; // Ensure there's enough space
    snprintf(newTextFile, sizeof(newTextFile), "%s.txt", filename);

    // Read the new text content from the reference file
    char *newText = readFileContent(newTextFile);
    if (newText == NULL) {
        fprintf(stderr, "File referensi gagal dibuka.\n");
        return;
    }

    // Process the new text to update the linked list
    processText(head, newText);

    // Free the allocated memory for the new text
    free(newText);
}