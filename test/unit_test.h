#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include "../lib/unity/src/unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void);
void tearDown(void);
void test_hello_world(void);
void test_delete_word_from_dictionary(void);
void test_add_word_to_dictionary(void);
void test_delete_non_existent_word(void);

#endif
