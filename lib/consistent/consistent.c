#include "consistent.h"

longest_sequence *find_seq(char *array, size_t arr_size) {
    if (array == NULL)
        return NULL;

    size_t beg_longest_seq = 0;
    size_t longest_seq_size = 0;
    size_t beg_cur_seq = 0;
    size_t cur_seq_size = 0;
    bool opened_quote = false;
    bool quotes = false;
    for (size_t i = 0; i < arr_size; ++i) {
        if (array[i] == 34) {
            if (opened_quote) {
                cur_seq_size = i - beg_cur_seq;
                opened_quote = false;
                if (longest_seq_size < cur_seq_size) {
                    beg_longest_seq = beg_cur_seq;
                    longest_seq_size = cur_seq_size;
                }
            }
            if (i + 1 < arr_size && array[i + 1] > 64 && array[i + 1] < 91) {
                beg_cur_seq = i + 1;
                opened_quote = true;
            }
            quotes = true;
        }
    }
    longest_sequence *seq = (longest_sequence *)malloc(sizeof(longest_seq_size));
    seq->size = longest_seq_size;
    if (longest_seq_size == 0)
        seq->array = NULL;
    else
        seq->array = (char *)malloc(longest_seq_size);
    if (array[arr_size - 1] == 34)
        opened_quote = true;
    seq->opened_quote = opened_quote;
    seq->quotes_in_array = quotes;
    memcpy(seq->array, array + beg_longest_seq, longest_seq_size);
    return seq;
}


void free_longest_seq(longest_sequence *seq) {
    if (seq == NULL)
        return;
    if(seq->array != NULL)
        free(seq->array);
    free(seq);
}

void print_longest_seq(longest_sequence *seq) {
    if (seq == NULL || seq->size == 0) {
        printf("Empty sequence\n");
        return;
    }
    printf("%s", seq->array);
}