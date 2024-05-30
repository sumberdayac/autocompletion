#include "unit_test.h"

#include "../lib/unity/src/unity.h"
#include "../lib/unity/src/unity.c"

#include "../include/fileProcess.h"
#include "../include/autocomplete.h"

vocabNode *createNodeDictionary(const char *word, int weight)
{
    vocabNode *newNode = (vocabNode *)malloc(sizeof(vocabNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    newNode->word = strdup(word);
    if (newNode->word == NULL)
    {
        fprintf(stderr, "Memory allocation for word failed.\n");
        free(newNode);
        exit(1);
    }
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void addWordDictionary(vocabNode **head, const char *word)
{
    vocabNode *foundNode = findWord(*head, word);
    if (foundNode)
    {
        foundNode->weight += 1;
    }
    else
    {
        vocabNode *newNode = createNodeDictionary(word, 1);
        newNode->next = *head;
        *head = newNode;
    }
}

void loadInitialData(vocabNode **head, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *equalSign = strchr(line, '=');
        if (equalSign)
        {
            *equalSign = '\0';
            char *word = line;
            int weight = atoi(equalSign + 1);
            addWordDictionary(head, word);
            findWord(*head, word)->weight = weight;
        }
    }

    fclose(file);
}

vocabNode *findWord(vocabNode *head, const char *word)
{
    vocabNode *current = head;
    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void deleteWordFromDictionary(vocabNode **head, const char *word, bool *found)
{
    vocabNode *current = *head;
    vocabNode *prev = NULL;

    while (current != NULL && strcmp(current->word, word) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        return;
    }

    if (prev == NULL)
    {
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    free(current->word);
    free(current);
    *found = true;
}

void saveData(vocabNode *head, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing.\n");
        exit(1);
    }

    vocabNode *current = head;
    while (current != NULL)
    {
        fprintf(file, "%s=%d\n", current->word, current->weight);
        current = current->next;
    }

    fclose(file);
}

void setUp(void)
{
    // This function is called before each test
}

void tearDown(void)
{
    // This function is called after each test
}

void test_hello_world(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello, World!", "Hello, World!");
}

void test_delete_word_from_dictionary(void)
{
    vocabNode *head = NULL;

    // Hardcode the initial data
    addWordDictionary(&head, "testword");
    addWordDictionary(&head, "anotherword");

    // Ensure 'testword' is in the dictionary
    vocabNode *foundNode = findWord(head, "testword");
    TEST_ASSERT_NOT_NULL(foundNode);

    bool found = false;
    deleteWordFromDictionary(&head, "testword", &found);
    TEST_ASSERT_TRUE(found);

    foundNode = findWord(head, "testword");
    TEST_ASSERT_NULL(foundNode);
}

void test_add_word_to_dictionary(void)
{
    vocabNode *head = NULL;

    // Add a new word to the dictionary
    addWordDictionary(&head, "newword");

    // Verify that the word is added
    vocabNode *foundNode = findWord(head, "newword");
    TEST_ASSERT_NOT_NULL(foundNode);
    TEST_ASSERT_EQUAL_STRING("newword", foundNode->word);
    TEST_ASSERT_EQUAL_INT(1, foundNode->weight);

    // Add the same word again to increment its weight
    addWordDictionary(&head, "newword");

    // Verify that the weight of the word is incremented
    foundNode = findWord(head, "newword");
    TEST_ASSERT_NOT_NULL(foundNode);
    TEST_ASSERT_EQUAL_STRING("newword", foundNode->word);
    TEST_ASSERT_EQUAL_INT(2, foundNode->weight);

    // Cleanup
    deleteWordFromDictionary(&head, "newword", &(bool){false});
}

void test_delete_non_existent_word(void)
{
    vocabNode *head = NULL;

    // Add some initial words to the dictionary
    addWordDictionary(&head, "word1");
    addWordDictionary(&head, "word2");

    // Attempt to delete a word that does not exist
    char word[] = "nonexistentword";
    bool found = false;
    deleteWordFromDictionary(&head, word, &found);

    // This assertion is incorrect, since "nonexistentword" should not be found in the dictionary
    vocabNode *foundNode = findWord(head, word);
    TEST_ASSERT_NOT_NULL(foundNode); // This will fail, as foundNode should be NULL

    // Cleanup
    deleteWordFromDictionary(&head, "word1", &(bool){false});
    deleteWordFromDictionary(&head, "word2", &(bool){false});
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hello_world);
    RUN_TEST(test_add_word_to_dictionary);
    RUN_TEST(test_delete_word_from_dictionary);
    RUN_TEST(test_delete_non_existent_word); // This test is expected to fail
    return UNITY_END();
}
