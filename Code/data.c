#include "srdefs.h"
#include "data.h"

void List_Init ( List *list, void ( *Print_Data )( void *data ), void ( *Destroy_Node )( void *data ) ){
    list->size = 0;
    list->Destroy_Node = Destroy_Node;
    list->Print_Data = Print_Data;
    list->head = NULL;
    list->tail = NULL;
}

void List_Destroy ( List *list ){
    while(list->size > 0){
        if(list->tail == NULL || List_Size(list) == 0)
            break;

        List_Pop(list);
    }

    memset(list, 0, sizeof(List));
}

void List_Push ( List *list, void *data ){
    node_t *new_node;

    if( (new_node = (node_t*)malloc(sizeof(node_t))) == NULL ){
        printf("Error: Can not create new list node!");
        return;
    }
    new_node->data = data;

    if(list->size == 0){
        list->head = new_node;
        list->head->prev = NULL;
        list->head->next = NULL;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->next = NULL;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
    //list->Print_Data(data);
    list->size++;
}

void List_Pop ( List *list ){
    if(list->size == 0){
        printf("Warning: List is empty, nothing to remove!\n");
        return;
    } else {
        node_t *curr = list->tail;
        if (list->size == 1){
            list->Destroy_Node(curr->data);
            free(curr);
            list->head = list->tail = NULL;
        } else {
            list->tail = curr->prev;
            list->tail->next = NULL;
            list->Destroy_Node(curr->data);
            free(curr);
        }
        list->size--;
    }
}

void List_Print ( List *list ){
    if(list->size == 0){
        printf("List is empty!\n");
        return;
    }
    node_t *curr = list->head;

    while( curr->next != NULL ){
        list->Print_Data(curr->data);
        curr = curr->next;
    }
}

int List_IsEmpty ( List *list ){
    if(list->size)
        return 1;
    else
        return 0;
}
