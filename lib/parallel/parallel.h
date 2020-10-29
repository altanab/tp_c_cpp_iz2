#ifndef TP_C_CPP_IZ2_PARALLEL_H
#define TP_C_CPP_IZ2_PARALLEL_H

#include <pthread.h>
#include "consistent.h"

typedef struct {
    char *array;
    size_t beg_index;
    size_t part_size;
    longest_sequence *seq;
}pthread_data;

longest_sequence *find_seq_in_parallel(char *array, size_t size, size_t num_threads);
void *find_longest_seq(void *arg);
longest_sequence *longest_seq_in_pthread_data(pthread_data *thread_data, size_t num_threads);
longest_sequence *check_longest(char *array, size_t beg_index, size_t size, longest_sequence *longest_seq);
#endif
