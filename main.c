#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "sharedressource.h"

/*
* Function used to put a new value in the shared buffer.
*
* @buf : the shared buffer to fill in with the adresses pointing to the data_t's
* @len : the length of the shared buffer
* @first : the pointer to the array index where you can find the first inserted element that's still in the buffer
*         (or more exactly the pointer to the first element, **if any**)
* @last : the pointer to the array index where you can find the first empty space in the buffer
*         (or more exactly the first NULL pointer in the array, **if any**)
* @in : the number of data_t* pointers in the buffer
* @d : the data_t* that has to be inserted in the buffer
*
* @return 0 if no error, -1 otherwise
*/
int put(data_t** buf, int len, int* first, int* last, int* in, data_t* d){
  if ( buf == NULL || last == NULL || first == NULL || in == NULL || d == NULL || len <= 0) return -1;
  int offset_last = * last;
  if ( offset_last < 0 || offset_last >= len) return -1;
  if ( * in < 0 || * in >= len) return -1;
  buf[offset_last] = d;
  if ( * in == 0 ) * first = * last;
  (* in)++;
  * last = ((* last)+1)%len;
  return 0;
}

/*
* Function used to get a value from the shared buffer.
*
* @buf : the shared buffer to fill out
* @len : the length of the shared buffer
* @first : the pointer to the array index where you can find the first element inserted that's still in the buffer
*         (or more exactly the pointer to the first element, **if any**)
* @last : the pointer to the array index where you can find the first empty space in the buffer
*         (or more exactly the first NULL pointer in the array, **if any**)
* @in : the number of data_t* pointers in the buffer
*
* @return the pointer to the element that you get if no error, NULL otherwise
*/
data_t* get(data_t** buf, int len, int* first, int* last, int* in){
    if ( buf == NULL || last == NULL || first == NULL || in == NULL || len <= 0) return NULL;
    int offset_first = * first;
    if ( offset_first < 0 || offset_first >= len) return NULL;
    if ( * in <= 0 || * in > len) return NULL;
    if ( * in == len ) * last = * first;
    data_t* return_ptr= buf[offset_first];
    buf[offset_first] = NULL;
    * first = ((* first)+1)%len;
    (* in)--;
    /*
    if ( * in == 0 ) {
      * first = 0;
      * last = 0;
    }
    */
    return return_ptr;
}

int start_consumer(data_parameter_t * threaddata) {
    data_t * ptr_buffer = threaddata->ptr_buffer;
    int buffer_size = BUFFER_SIZE;
    int data_size = DATA_SIZE;
    for (int i=0; i < data_size; i++) {
        usleep(((rand() % (SLEEP_TIME_IN_SECONDS)) + 1) * 100000);
        int rc = 0;
        int nbr_elems_in_buffer = 0;
        if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
        nbr_elems_in_buffer = * threaddata->in;
        if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        while ( nbr_elems_in_buffer ==  0) {
            usleep(((rand() % (SLEEP_TIME_IN_SECONDS)) + 1) * 100000);
            if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
            nbr_elems_in_buffer = * threaddata->in;
            if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        }
        if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
        data_t* ptr_fetched = get(&ptr_buffer, buffer_size, threaddata->first, threaddata->last, threaddata->in);
        if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        printf("Consumer Longitude=%d Latitude=%d Average Temperature=%2.2f\n", ptr_fetched->longitude, ptr_fetched->latitude, ptr_fetched->avg_temp);
    }
    return 0;
}

int start_producer(data_parameter_t * threaddata) {
    data_t * ptr_data = threaddata->ptr_data;
    data_t * ptr_buffer = threaddata->ptr_buffer;
    int buffer_size = BUFFER_SIZE;
    int data_size = DATA_SIZE;
    for (int i=0; i < data_size; i++) {
        usleep(((rand() % (SLEEP_TIME_IN_SECONDS)) + 1) * 100000);
        data_t* ptr = ptr_data+i;
        int rc = 0;
        int nbr_elems_in_buffer = 0;
        if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
        nbr_elems_in_buffer = * threaddata->in;
        if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        while ( nbr_elems_in_buffer ==  buffer_size) {
            usleep(((rand() % (SLEEP_TIME_IN_SECONDS)) + 1) * 100000);
            if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
            nbr_elems_in_buffer = * threaddata->in;
            if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        }
        if (0 != (rc = pthread_mutex_lock(threaddata->ptr_mutex))) return -1;
        if (0 != (rc = put(&ptr_buffer, buffer_size, threaddata->first, threaddata->last, threaddata->in, ptr))) return -1;
        if (0 != (rc = pthread_mutex_unlock(threaddata->ptr_mutex))) return -1;
        printf("Producer Longitude=%d Latitude=%d Average Temperature=%2.2f\n", ptr->longitude, ptr->latitude, ptr->avg_temp);
    }
    return 0;
}

int main()
{
    int buffer_size = BUFFER_SIZE;
    int nbr_elems_in_buffer = 0;

    data_t * arr_data = (data_t*)malloc(sizeof(data_t)*DATA_SIZE);
    for ( int i=0; i <DATA_SIZE; i++) {
        data_t* current = arr_data+i;
        current->latitude = 100+i;
        current->longitude = 100+i;
        current->avg_temp = ((float)i/10)+10;
    }
    data_t * ptr_buffer = (data_t*)malloc(sizeof(data_t *)*BUFFER_SIZE);
    int first = 0, last = 0, rc = 0;

    rc = unittest(&first, &last, &nbr_elems_in_buffer, &buffer_size, arr_data);

    rc = test_with_2threads(&first, &last, &nbr_elems_in_buffer, &buffer_size, arr_data, ptr_buffer);
    if ( rc == 0 ) printf("Test Consumer Producer with threads succeeded\n");


    free(arr_data);
    free(ptr_buffer);

    return rc;
}


int test_with_2threads(int * first, int * last, int * nbr_elems_in_buffer, int * buffer_size, data_t * arr_data, data_t * ptr_buffer) {

    pthread_t thread1, thread2;
    int rc = 0;
    int * ptr_rc_thread = NULL;
    int * ptr_first = first;
    int * ptr_last = last;
    pthread_mutex_t mutex;
    if (0 != (rc = pthread_mutex_init(&mutex, NULL))) return -1;

    data_parameter_t data_parameter = {arr_data, ptr_buffer, buffer_size , ptr_first, ptr_last, nbr_elems_in_buffer, &mutex};
    data_parameter_t * ptr_data_parameter = &data_parameter;

    if ( 0 != (rc=pthread_create(&thread1, NULL, (void *)start_producer, ptr_data_parameter))) return -1;
    if ( 0 != (rc=pthread_create(&thread2, NULL, (void *)start_consumer, ptr_data_parameter))) return -1;

    if ( 0 != (rc=pthread_join(thread1, (void **)&ptr_rc_thread))) return -1;
    if ( 0 != (rc=pthread_join(thread2, (void **)&ptr_rc_thread))) return -1;
    return 0;
}
