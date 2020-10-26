#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <dlfcn.h>
#include "consistent.h"

double time_exec_consistently(char *array, size_t size);
double time_exec_in_parallel(char *array, size_t size);

bool init_arr(char *arr, size_t size_arr){
    if(!arr){
        return false;
    }
    for(int i = 0; i < size_arr; ++i){
        arr[i] = rand() % 127;
    }
    return true;

}

int main() {

    size_t size = 1024 * 1024 * 100; // 100 mB
    char *array = (char *)malloc(size);
    if (!init_arr(array, size)) {
        free(array);
        return 1;
    }
    printf("%lf\n", time_exec_consistently(array, size));
    printf("%lf\n", time_exec_in_parallel(array, size));

    free(array);
}

double time_exec_consistently(char *array, size_t size) {
    struct timeval stop, start;
    double mkrSec = 1000000.0;

    gettimeofday(&start, NULL);
    longest_sequence *longest_seq = find_seq(array, size);
    gettimeofday(&stop, NULL);

    free_longest_seq(longest_seq);

    return (double)(stop.tv_usec - start.tv_usec) / mkrSec;
}

double time_exec_in_parallel(char *array, size_t size) {
    void *library = dlopen("libtp_c_cpp_iz2_dlib.so", RTLD_LAZY);
    if (!library) {
        printf("no library");
        return 1;
    }
    longest_sequence *(*find_seq_in_parallel)(char *array, size_t size, size_t num_threads) = dlsym(library, "find_seq_in_parallel");

    struct timeval stop, start;
    double mkrSec = 1000000.0;

    gettimeofday(&start, NULL);
    longest_sequence *longest_seq_in_parallel = find_seq_in_parallel(array, size, get_nprocs());
    gettimeofday(&stop, NULL);

    free_longest_seq(longest_seq_in_parallel);

    dlclose(library);

    return (double)(stop.tv_usec - start.tv_usec) / mkrSec;
}
