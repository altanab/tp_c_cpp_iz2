#ifndef TP_C_CPP_IZ2_CONSISTENT_H
#define TP_C_CPP_IZ2_CONSISTENT_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char *array;
    size_t size;
    bool opened_quote;
    bool quotes_in_array;
} longest_sequence;


longest_sequence  *create_seq(char *array, size_t seq_size, bool opened_q, bool quotes);
longest_sequence *find_seq(char *array, size_t arr_size);
void free_longest_seq(longest_sequence *seq);
void print_longest_seq(longest_sequence *seq);
#endif