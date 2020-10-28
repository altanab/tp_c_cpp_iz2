#include "parallel.h"


longest_sequence *find_seq_in_parallel(char *array, size_t size, size_t num_threads) {
    if (array == NULL)
        return NULL;

    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL)
        return NULL;

    pthread_data* thread_data = (pthread_data*)malloc(num_threads * sizeof(pthread_data));
    if (thread_data == NULL) {
        free (threads);
        return NULL;
    }

    size_t part_size = size / num_threads;
    size_t remainder = size % num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        thread_data[i].array = array;
        thread_data[i].beg_index = i * part_size;
        if (i + 1 == num_threads)
            part_size += remainder;
        thread_data[i].part_size = part_size;
        thread_data[i].seq = NULL;

        if (pthread_create(&(threads[i]), NULL, find_longest_seq, (&thread_data[i])) != 0) {
            free(threads);
            free(thread_data);
            return NULL;
        }
    }

    for (size_t i = 0; i < num_threads; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            free(threads);
            free(thread_data);
            return NULL;
        }
    }
    free(threads);

    longest_sequence *longest_seq = longest_seq_in_pthread_data(thread_data, num_threads);
    free(thread_data);

    return longest_seq;
}

void *find_longest_seq(void *arg) {
    pthread_data  *thread_data = (pthread_data *)arg;
    if (thread_data == NULL)
        return NULL;

    thread_data->seq = find_seq(thread_data->array + thread_data->beg_index, thread_data->part_size);
    return NULL;
}

longest_sequence *longest_seq_in_pthread_data(pthread_data *thread_data, size_t num_threads) {
    longest_sequence *longest_seq = create_seq(NULL, 0, false, false);
    for (size_t i = 0; i < num_threads; ++i) {
        if (thread_data[i].seq->size > longest_seq->size) {
            free_longest_seq(longest_seq);
            longest_seq = thread_data[i].seq;
        }
        if (thread_data[i].seq->opened_quote) {
            size_t extra_part_size = thread_data[i].part_size;
            for (size_t j = i + 1; j < num_threads; ++j) {
                extra_part_size += thread_data[j].part_size;
                if (thread_data[j].seq->quotes_in_array) {
                    longest_seq = check_longest(thread_data[j].array, thread_data[i].beg_index, extra_part_size, longest_seq);
                    break;
                }
            }
        }
        if (thread_data[i].seq != longest_seq)
            free_longest_seq(thread_data[i].seq);

        thread_data[i].seq = NULL;
    }
    return longest_seq;
}

longest_sequence *check_longest(char *array, size_t beg_index, size_t size, longest_sequence *longest_seq) {
    longest_sequence *new_seq = find_seq(array + beg_index, size);
    if (longest_seq == NULL)
        return new_seq;
    if (new_seq->size > longest_seq->size) {
        free_longest_seq(longest_seq);
        return new_seq;
    }
    free_longest_seq(new_seq);
    return longest_seq;
}
