/* Singly linked list abstract data type */
#include <stdio.h>
#include <string.h>

// Node for a singly linked list
typedef struct SinglyLinkedListNode_ {
  void *data;
  struct SinglyLinkedListNode_ *next;
} SLLNode;

// A structure representing a singly linked list
typedef struct SinglyLinkedList_ {
  size_t size;
  SLLNode *tail;
  SLLNode *head;
  void (*destroy)(void *data);
} SLList;

/* Public interface for the ADT */
// Function forward declarations
void sl_list_init(SLList *list, void (*destroy)(void *data));
void sl_list_destroy(SLList *list);
int sl_list_ins_next(SLList *list, SLLNode *node, const void *data);
int sl_list_rem_next(SLList *list, SLLNode *node, void **data);

// Macros
#define sl_list_size(list) ((list)->size)
#define sl_list_tail(list) ((list)->tail)
#define sl_list_head(list) ((list)->head)
#define sl_list_is_tail(node) ((node)->next == NULL ? 1: 0)
#define sl_list_is_head(list, node) ((node) == (list)->head ? 1: 0)
#define sl_list_data(node) ((node)->data)
#define sl_list_next(node) ((node)->next)


// Function implementations
void sl_list_init(SLList *list, void (*destroy)(void *data)) {
  printf("Initializing list... ");
  list->size = 0;
  list->tail = NULL;
  list->head = NULL;
  list->destroy = destroy;
  puts("Initialization completed.");
return;
}

void sl_list_destroy(SLList *list) {
  void *deleted_data;

  puts("List destruction in progress... ");
  while( sl_list_size(list) > 0 ) {
    if( (sl_list_rem_next(list, NULL, (void **)&deleted_data) == 0) && (list->destroy != NULL) ) {
      printf("A node has been removed... ");
      list->destroy(deleted_data);
      puts("The node's data has been released."); 
    }
  }

  memset(list, 0, sizeof(SLList));
  puts("List destruction completed.");

return;
}

int sl_list_ins_next(SLList *list, SLLNode *node, const void *data) {
  //Allocate memory for the new node
  SLLNode *new_node;
  if( (new_node = (SLLNode *)malloc(sizeof(SLLNode))) == NULL ) {
    puts("Could not allocate memory for a new node.");
    return -1;
  }
  new_node->data = (void *)data;

  //Inserting new node at the head of list
  if( node == NULL ) {
    printf("Inserting new node at the head of the list... ");
    if( sl_list_size(list) == 0 ) {
      list->tail = new_node;
    }
    new_node->next = list->head;
    list->head = new_node;
  }
  //Inserting new node elsewhere
  else {
    printf("Inserting a new node in the list... ");
    if( node->next == NULL ) {
      list->tail = new_node;
      printf("The list will have a new tail... ");
    }
    new_node->next = node->next;
    node->next = new_node;
  }

  puts("Insertion completed.");
  list->size++;

return 0;
}

int sl_list_rem_next(SLList *list, SLLNode *node, void **data) {
  //Cannot remove a node from an empty list
  if( sl_list_size(list) == 0 ) {
    puts("Cannot remove a node from an empty list.");
    return -1;
  }
  
  SLLNode *deleted_node;

  //Sentinal value is NULL for second argument, indicates removal of head node
  if( node == NULL ) {
    printf("Deletion of head node in progress... ");
    *data = list->head->data;
    deleted_node = list->head;
    list->head = list->head->next;
    
    if( sl_list_size(list) == 1 ) {
      list->tail = NULL;
    }
  }
  //Otherwise, the node is being removed from elsewhere in the list
  else {
    //Cannot remove a node that is after the tail of the list, it doesn't exist
    if( node->next == NULL ) {
      puts("Cannot remove a node that is after the tail of the list.");
      return -1;
    }

    printf("Deletion of arbitrary node in progress... ");
    *data = node->next->data;
    deleted_node = node->next;

    node->next = node->next->next;
    if( node->next == NULL ) {
      list->tail = node;
    }
  }

  free(deleted_node);
  puts("Node deleted.");

  list->size--;

return 0;
}

