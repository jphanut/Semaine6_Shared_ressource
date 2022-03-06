#include "sharedressource.h"
#include "CUnit/Basic.h"

data_t ** ptr_buffer = NULL;
int * g_first = NULL;
int * g_last = NULL;
int * g_nbr_elems_in_buffer = NULL;
int * g_buffer_size = NULL;
data_t * g_arr_data = NULL;


int init_suite1(void)
{
    ptr_buffer = (data_t **)malloc(BUFFER_SIZE*sizeof(data_t *));
    if ( ptr_buffer == NULL ) {
        return -1;
    }
    else {
        return 0;
    }
}

void test_slotinbuffernull() {
    data_t ** ptr_slots = ptr_buffer;
    for (int i=0; i < BUFFER_SIZE; i++) CU_ASSERT(NULL == ptr_slots[i]);
}

void test_insert_items_0_1_2() {
    data_t ** ptr_slots = ptr_buffer;
    int rc = 0;
    CU_ASSERT(0 == * g_nbr_elems_in_buffer);
    CU_ASSERT(0 == * g_first);
    CU_ASSERT(0 == * g_last);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+0);
    CU_ASSERT(rc == 0);
    CU_ASSERT(1 == * g_nbr_elems_in_buffer);
    CU_ASSERT(0 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+0);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+1);
    CU_ASSERT(rc == 0);
    CU_ASSERT(2 == * g_nbr_elems_in_buffer);
    CU_ASSERT(0 == * g_first);
    CU_ASSERT(2 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+0);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+2);
    CU_ASSERT(rc == 0);
    CU_ASSERT(3 == * g_nbr_elems_in_buffer);
    CU_ASSERT(0 == * g_first);
    CU_ASSERT(3 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+0);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
}

void test_remove_items_0() {
    data_t ** ptr_slots = ptr_buffer;
    data_t * d = NULL;
    int rc = 0;
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+0);
    CU_ASSERT(2 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(3 == * g_last);
    CU_ASSERT(ptr_slots[0] == NULL);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
}

void test_insert_items_3_4_5() {
    data_t ** ptr_slots = ptr_buffer;
    int rc = 0;
    CU_ASSERT(2 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(3 == * g_last);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+3);
    CU_ASSERT(rc == 0);
    CU_ASSERT(3 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(4 == * g_last);
    CU_ASSERT(ptr_slots[0] == NULL);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == NULL);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+4);
    CU_ASSERT(rc == 0);
    CU_ASSERT(4 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(0 == * g_last);
    CU_ASSERT(ptr_slots[0] == NULL);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+5);
    CU_ASSERT(rc == 0);
    CU_ASSERT(5 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
}

void test_insert_item_6_buffer_full() {
    data_t ** ptr_slots = ptr_buffer;
    int rc = 0;
    CU_ASSERT(5 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(1 == * g_last);
    rc = put(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer, g_arr_data+6);
    CU_ASSERT(rc == -1);
    CU_ASSERT(5 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == g_arr_data+1);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
}

void get_all_iteams_in_buffer() {
    data_t ** ptr_slots = ptr_buffer;
    int rc = 0;
    data_t * d = NULL;
    CU_ASSERT(5 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(1 == * g_last);
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+1);
    CU_ASSERT(4 == * g_nbr_elems_in_buffer);
    CU_ASSERT(2 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == g_arr_data+2);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+2);
    CU_ASSERT(3 == * g_nbr_elems_in_buffer);
    CU_ASSERT(3 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == g_arr_data+3);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+3);
    CU_ASSERT(2 == * g_nbr_elems_in_buffer);
    CU_ASSERT(4 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == g_arr_data+4);
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+4);
    CU_ASSERT(1 == * g_nbr_elems_in_buffer);
    CU_ASSERT(0 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == g_arr_data+5);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
    d = get(ptr_slots, BUFFER_SIZE, g_first, g_last, g_nbr_elems_in_buffer);
    CU_ASSERT(rc == 0);
    CU_ASSERT(d == g_arr_data+5);
    CU_ASSERT(0 == * g_nbr_elems_in_buffer);
    CU_ASSERT(1 == * g_first);
    CU_ASSERT(1 == * g_last);
    CU_ASSERT(ptr_slots[0] == NULL);
    CU_ASSERT(ptr_slots[1] == NULL);
    CU_ASSERT(ptr_slots[2] == NULL);
    CU_ASSERT(ptr_slots[3] == NULL);
    CU_ASSERT(ptr_slots[4] == NULL);
}

int clean_suite1(void)
{
    free(ptr_buffer );
    return 0;
}

int unittest (int * first, int * last, int * nbr_elems_in_buffer, int * buffer_size, data_t * arr_data) {
   g_first = first;
   g_last = last;
   g_nbr_elems_in_buffer = nbr_elems_in_buffer;
   g_buffer_size = buffer_size;
   g_arr_data = arr_data;
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }
   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "test all slots in buffer null", test_slotinbuffernull)) ||
       (NULL == CU_add_test(pSuite, "test insert of 3 first elements", test_insert_items_0_1_2)) ||
       (NULL == CU_add_test(pSuite, "test remove first element", test_remove_items_0)) ||
       (NULL == CU_add_test(pSuite, "test insert of 3 next elements", test_insert_items_3_4_5)) ||
       (NULL == CU_add_test(pSuite, "test insert of 6th item when buffer full", test_insert_item_6_buffer_full)) ||
       (NULL == CU_add_test(pSuite, "test get the 5 items in the buffer", get_all_iteams_in_buffer))
   )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }


   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
