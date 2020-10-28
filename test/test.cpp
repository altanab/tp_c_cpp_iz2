#include <gtest/gtest.h>
#include <sys/sysinfo.h>
#include <dlfcn.h>
#include <string.h>

extern "C" {
#include "consistent.h"
}

longest_sequence *execute_parallel(char *array, size_t size) {
    void *library = dlopen("libtp_c_cpp_iz2_dlib.so", RTLD_LAZY);
    if (!library) {
        printf("no library");
        return NULL;
    }
    longest_sequence *(*find)(char *array, size_t size, size_t num_threads) =
            reinterpret_cast<longest_sequence *(*)(char*, size_t, size_t)>(dlsym(library, "find_seq_in_parallel"));

    longest_sequence *longest_seq_in_parallel = find(array, size, get_nprocs());

    dlclose(library);

    return longest_seq_in_parallel;
}


TEST(test, SimpleTest) {
    size_t arr_size = 16;
    char input_array[16] = "hello\"World\"Bye";
    char *test_array = (char *)malloc(arr_size);
    memcpy(test_array, input_array, arr_size);
    size_t seq_size = 5;
    char *expected_seq = (char *)malloc(seq_size);
    memcpy(expected_seq, input_array + 6, seq_size);

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(0, memcmp(expected_seq, consistent_result->array, seq_size));
    EXPECT_EQ(seq_size, consistent_result->size);
    EXPECT_EQ(0,memcmp(expected_seq, parallel_result->array, seq_size));
    EXPECT_EQ(seq_size, parallel_result->size);

    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(expected_seq);
    free(test_array);
}

TEST(test, NoQuotesTest) {
    size_t arr_size = 16;
    char input_array[16] = "helloAWorldqBye";
    char *test_array = (char *)malloc(arr_size);
    strcpy(test_array, input_array);

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(NULL, consistent_result->array);
    EXPECT_EQ(NULL, parallel_result->array);

    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(test_array);
}

TEST(test, NULLTest) {
    size_t arr_size = 16;
    char *test_array = NULL;

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(NULL, consistent_result);
    EXPECT_EQ(NULL, parallel_result);
}

TEST(test, OnlyQuotesTest) {
    size_t arr_size = 16;
    char *test_array = (char *)malloc(arr_size);
    for (size_t i = 0; i < arr_size; ++i) {
        test_array[i] = 34;
    }

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(NULL, consistent_result->array);
    EXPECT_EQ(NULL, parallel_result->array);


    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(test_array);
}

TEST(test, SeveralQuotesTest) {
    size_t arr_size = 20;
    char input_array[20] = "hell\"World\"Bye\"Yes\"";
    char *test_array = (char *)malloc(arr_size);
    memcpy(test_array, input_array, arr_size);
    size_t seq_size = 5;
    char *expected_seq = (char *)malloc(seq_size);
    memcpy(expected_seq, input_array + 5, seq_size);

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(0, memcmp(expected_seq, consistent_result->array, seq_size));
    EXPECT_EQ(seq_size, consistent_result->size);
    EXPECT_EQ(0,memcmp(expected_seq, parallel_result->array, seq_size));
    EXPECT_EQ(seq_size, parallel_result->size);

    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(expected_seq);
    free(test_array);
}

TEST(test, NoUpperCaseTest) {
    size_t arr_size = 16;
    char input_array[16] = "hello\"world\"bye";
    char *test_array = (char *)malloc(arr_size);
    memcpy(test_array, input_array, arr_size);

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(NULL, consistent_result->array);
    EXPECT_EQ(NULL, parallel_result->array);

    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(test_array);
}

TEST(test, SeveralQuotesTest2) {
    size_t arr_size = 20;
    char input_array[20] = "hell\"Wor\"Byekl\"Yes\"";
    char *test_array = (char *)malloc(arr_size);
    memcpy(test_array, input_array, arr_size);
    size_t seq_size = 5;
    char *expected_seq = (char *)malloc(seq_size);
    memcpy(expected_seq, input_array + 9, seq_size);

    longest_sequence *consistent_result = find_seq(test_array, arr_size);
    longest_sequence *parallel_result = execute_parallel(test_array, arr_size);

    EXPECT_EQ(0, memcmp(expected_seq, consistent_result->array, seq_size));
    EXPECT_EQ(seq_size, consistent_result->size);
    EXPECT_EQ(0,memcmp(expected_seq, parallel_result->array, seq_size));
    EXPECT_EQ(seq_size, parallel_result->size);

    free_longest_seq(consistent_result);
    free_longest_seq(parallel_result);
    free(expected_seq);
    free(test_array);
}