

#ifndef SHAREDRESSOURCE_H_INCLUDED
#define SHAREDRESSOURCE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define DATA_SIZE 20
#define BUFFER_SIZE 5
#define SLEEP_TIME_IN_SECONDS 1

typedef struct data {
    int longitude;
    int latitude;
    float avg_temp;
} data_t;

typedef struct data_parameter {
    data_t * ptr_data;
    data_t * ptr_buffer;
    int * len;
    int * first;
    int * last;
    int * in;
    pthread_mutex_t * ptr_mutex;
} data_parameter_t;

int put(data_t** buf, int len, int* first, int* last, int* in, data_t* d);
data_t* get(data_t** buf, int len, int* first, int* last, int* in);

int unittest (int * first, int * last, int * nbr_elems_in_buffer, int * buffer_size, data_t * arr_data);
int test_with_2threads(int * first, int * last, int * nbr_elems_in_buffer, int * buffer_size, data_t * arr_data, data_t * ptr_buffer);

#endif // SHAREDRESSOURCE_H_INCLUDED
