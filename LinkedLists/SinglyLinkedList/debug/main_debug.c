/*
 * Testing the singly linked list abstract data type.
 * Unity build
 *
 * Flags:
 * -std=c99             : Use the best C standard
 * -fsanitize=address   : address sanitizer enabled
 * -Wall                : All warnings are displayed
 * -Werror              : All warnings lead to compile-time errors
 * -g3                  : Add the most debugger info to file
 * -O0                  : No compiler optimizations
 *
 * Compilation command:
 * gcc debug/main_debug.c -o debug/bin/main -std=c99 -fsanitize=address -Wall -Werror -g3
 *
 * Run command: ./debug/bin/main
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../src/singly_linked_list.c"

// Macro for safely freeing pointers, plus a forward declaration used by the macro
#define safe_free(ptr) safer_free((void **)&ptr)
void safer_free(void **ptr);

void print_formatting_func(void *data) {

  // Determine the size of the string needed for data 
  size_t size_of_formatted_string = snprintf(NULL, 0, "%d", *(int8_t *)data);
  // Allocate memory for that formatted string
  char *formatted_data_buffer = malloc(size_of_formatted_string + 1);
  // Write your formatted string to the buffer
  sprintf(formatted_data_buffer, "%d", *(int8_t *)data);
  //Print out that buffer with printf...
  printf("%s -> ", formatted_data_buffer);

  safe_free(formatted_data_buffer);
}

int main(void) {

  puts("\nSingly Linked List Abstract Data Type");

  // Allocate memory for the list structure
  SLList *my_list;
  if ( (my_list = (SLList *)(malloc(sizeof(SLList)))) == NULL ) {
    puts("Memory could not be allocated for list structure.");
    return -1;
  }

  // Testing the list initialization function.
  sl_list_init(my_list, free, print_formatting_func);
  assert(sl_list_size(my_list) == 0); 
  assert(sl_list_tail(my_list) == NULL);
  assert(sl_list_head(my_list) == NULL);


  // Declaring a pointer for integer data to be inserted into nodes/elements of the list
  int8_t *data;

  //allocating memory for each floating point value
  if ( (data = (int8_t *)malloc(sizeof(int8_t))) == NULL ) {
    puts("Failed to allocate memory for new datum.");
    return -1;
  }
  *data = 8;

  // Trying to remove element from an empty list returns value of -1
  assert(sl_list_rem_next(my_list, NULL, (void **)&data) == -1);

  // Inserting the first element
  assert(sl_list_ins_next(my_list, NULL, data) == 0);
  assert(sl_list_size(my_list) == 1); 

  // Confirming that the first element is both the tail and head of the list.
  assert(sl_list_is_tail(sl_list_head(my_list)) == 1);
  assert(sl_list_is_head(my_list, sl_list_head(my_list)) == 1);

  // Confirming the node contains the correct data
  assert( *(int8_t *)sl_list_data(sl_list_head(my_list)) == 8);

  /* Picture of linked list:
  
       Head -> [8] -> NULL
  */
  sl_list_print(my_list); 
  

  // Allocating memory for a new value
  if ( (data = (int8_t *)malloc(sizeof(int8_t))) == NULL ) {
    puts("Failed to allocate memory for new datum.");
    return -1;
  }
  // Assigning value to the pointer and creating new head element of the list
  *data = -50;
  assert(sl_list_ins_next(my_list, NULL, data) == 0);
  assert(sl_list_size(my_list) == 2);
  assert( *(int8_t *)sl_list_data(sl_list_head(my_list)) == -50);
  assert(sl_list_next(sl_list_tail(my_list)) == NULL);
  assert(sl_list_next(sl_list_head(my_list)) == sl_list_tail(my_list));
  assert(sl_list_next(sl_list_head(my_list)) != NULL);
   

  // Picture of linked list:
  // Head -> [-50] -> [8] -> NULL
  sl_list_print(my_list);

  // Allocating memory for a new value
  if ( (data = (int8_t *)malloc(sizeof(int8_t))) == NULL ) {
    puts("Failed to allocate memory for new datum.");
    return -1;
  } 
  // Assigning value to the pointer and creating new tail element of the list
  *data = 100;
  assert(sl_list_ins_next(my_list, sl_list_tail(my_list), data) == 0);
  assert(sl_list_size(my_list) == 3);
  assert( *(int8_t *)sl_list_data(sl_list_tail(my_list)) == 100);
  assert(sl_list_next(sl_list_tail(my_list)) == NULL);
  assert(sl_list_next(sl_list_head(my_list)) != NULL);
  

  // Picture of linked list:
  // Head -> [-50] -> [8] -> [100] -> NULL
  sl_list_print(my_list);

  // Allocating memory for a new value
  if ( (data = (int8_t *)malloc(sizeof(int8_t))) == NULL ) {
    puts("Failed to allocate memory for new datum.");
    return -1;
  }
  // Assigning value to the pointer and creating new element after the head of the list
  *data = -48;
  assert(sl_list_ins_next(my_list, sl_list_head(my_list), data) == 0);
  assert(sl_list_size(my_list) == 4);
  assert( *(int8_t *)sl_list_data(sl_list_next(sl_list_head(my_list))) == -48);
  assert(sl_list_next(sl_list_tail(my_list)) == NULL);
  assert(sl_list_next(sl_list_head(my_list)) != NULL);


  // Picture of linked list:
  // Head -> [-50] -> [-48] -> [8] -> [100] -> NULL
  sl_list_print(my_list);

  /* Removing elements manually */
  int8_t *value_removed;
  
  // Removing the head element of the list
  assert( sl_list_rem_next(my_list, NULL, (void **)&value_removed) == 0);
  assert( sl_list_size(my_list) == 3);
  assert( *value_removed == -50 );

  printf("Value removed from list: %d\n\n", *value_removed);
  safe_free(value_removed);


  // Picture of linked list:
  // Head -> [-48] -> [8] -> [100] -> NULL
  sl_list_print(my_list);

  // Removing after tail, should produce error message.
  assert(sl_list_rem_next(my_list, sl_list_tail(my_list), (void **)&value_removed) == -1);
  assert( sl_list_size(my_list) == 3);
  assert(sl_list_next(sl_list_tail(my_list)) == NULL);
  assert(sl_list_next(sl_list_head(my_list)) != NULL);


  // Removing the element after the head element
  assert(sl_list_rem_next(my_list, sl_list_head(my_list), (void **)&value_removed) == 0);
  assert( sl_list_size(my_list) == 2);
  assert( *value_removed == 8 );

  printf("Value removed from list: %d\n\n", *value_removed);
  safe_free(value_removed);

  // Picture of linked list:
  // Head -> [-48] -> [100] -> NULL
  sl_list_print(my_list);

  // Destroying the list after it is not needed.
  sl_list_destroy(my_list); //frees element structs, and data pointer
  safe_free(my_list); //frees list pointer
  safe_free(value_removed); //frees pointer that returned data stored by deleted nodes

return 0;
}

/* */
void safer_free(void **ptr) {
  if( ptr != NULL && *ptr != NULL ) {
    free(*ptr);
    *ptr = NULL;
  }
}
