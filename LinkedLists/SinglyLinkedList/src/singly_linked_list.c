/*******************************************************************************
 * Title: "Algorithms with C by Kyle Loudon (O'Reilly)
 * Author(s): Kyle Loudon
 * Date: August 1999
 * ISBN-13: 978-1-565-92453-6
 *
 * This code is based on the original implementation cited above.
 *
 * Modifications I made were renaming element to node, which is the more common
 * convention I see for the pieces of a linked list.
 *
 * I also included my own traverse-and-print function that allows the user to
 * define a format that is suitable for their data. This function is passed to
 * the list initialization function similar to the user-defined destroy function.
 *
 * Functions were prepended with sl or dl or cl to make clear that they are for
 * a sinly linked list, a doubly linked list, and circularly linked list, respectively.
 *******************************************************************************/




/* Singly linked list abstract data type */
#include <stdio.h>
#include <string.h> // For memset()

// Node for a singly linked list
typedef struct SinglyLinkedListNode_ {
  void                          *data;
  struct SinglyLinkedListNode_  *next;
} SLLNode;

// A structure representing a singly linked list
typedef struct SinglyLinkedList_ {
  size_t size;
  SLLNode *head;
  SLLNode *tail;
  void (*destroy)(void *data);
  void (*print_func)(void *data);
} SLList;

/* Public interface for the ADT */
// Function forward declarations
void sl_list_init(SLList *list, void (*destroy)(void *data), void (*print_formatting_func)(void *data));
void sl_list_destroy(SLList *list);
int sl_list_ins_next(SLList *list, SLLNode *node, void const *data);
int sl_list_rem_next(SLList *list, SLLNode *node, void **data);

// Macros
#define sl_list_size(list)          ((list)->size)
#define sl_list_head(list)          ((list)->head)
#define sl_list_tail(list)          ((list)->tail)
#define sl_list_is_head(list, node) ((node) == (list)->head ? 1 : 0)
#define sl_list_is_tail(node)       ((node)->next == NULL ? 1: 0)
#define sl_list_data(node)          ((node)->data)
#define sl_list_next(node)          ((node)->next)

// Function implementations
void sl_list_init(SLList *list, void (*destroy)(void *data), void (*print_formatting_func)(void *data)) {

  printf("Initializing a singly linked list...");
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  list->destroy = destroy;
  list->print_func = print_formatting_func;
  puts("Initialization complete.");

return;
}

void sl_list_destroy(SLList *list) {
  void *data;

  while( sl_list_size(list) > 0 ) {
    if( (sl_list_rem_next(list, NULL, (void **)&data) == 0) && (list->destroy != NULL) ) {
      list->destroy(data);
    }
  }

  memset(list, 0, sizeof(SLList));

return;
}

int sl_list_ins_next(SLList *list, SLLNode *node, void const *data) {

  // Creation and initialization of a new node
  SLLNode *new_node;
  if( (new_node = (SLLNode *)malloc(sizeof(SLLNode))) == NULL ) {
    puts("Could not allocate memory for a new node.");
    return -1;
  }
  new_node->data = (void *)data;

  // Insertion
  if( node == NULL ) {
    if( sl_list_size(list) == 0 ) {
      list->tail = new_node;
    }
    new_node->next = list->head;
    list->head = new_node;
  }
  else {
    if( node->next == NULL ) {
      list->tail = new_node;
    }
    new_node->next = node->next;
    node->next = new_node;
  }

  list->size++;

return 0;
}

int sl_list_rem_next(SLList *list, SLLNode *node, void **data) {
  
  // Logic check, cannot remove from an empty list
  if( sl_list_size(list) == 0 ) {
    puts("Cannot remove a node from an empty linked list.");
    return -1;
  }

  SLLNode *deleted_node;  

  if( node == NULL ) {
    *data = list->head->data;
    deleted_node = list->head;

    list->head = list->head->next;
    if( sl_list_size(list) == 1 ) {
      list->tail = NULL;
    }
  }
  else {
    // Logic check, cannot remove a node that comes after the tail of the linked list.
    if( node->next == NULL ) {
      puts("cannot remove a node that comes after the tail of the linked list.");
      return -1;
    }

    *data = node->next->data;
    deleted_node = node->next;

    node->next = node->next->next;
    if( node->next == NULL ) {
      list->tail = node;
    }
  }

  free(deleted_node);
  list->size--;

return 0;
}


/* This function is not included in the text cited at top of page */
void sl_list_print(SLList const *list) {
  SLLNode *node;
  printf("HEAD -> ");

  for( node = sl_list_head(list); node != NULL; node = sl_list_next(node) ) {
    void *data = sl_list_data(node);
    list->print_func(data);
  }

  printf("NULL/TAIL\n");
}
