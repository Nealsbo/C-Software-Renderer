#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

typedef struct node {
    void        *data;
    struct node *next, *prev;
} node_t;

typedef struct List_ {
    int     size;

    void    (*Print_Data)  (void *data);
    void    (*Destroy_Node)(void *data);

    node_t  *head;
    node_t  *tail;
} List;

void    List_Init      ( List *list, void ( *Print_Data )( void *data ), void ( *Destroy_Node )( void *data ) );
void    List_Destroy   ( List *list );
void    List_Push      ( List *list, void *data );
void    List_Pop       ( List *list );
node_t *List_GetByIndex( List *list, int index );
void    List_Print     ( List *list );
int     List_IsEmpty   ( List *list );

#define List_Head(list)   ((list)->head)
#define List_Tail(list)   ((list)->tail)
#define List_Size(list)   ((list)->size)
#define List_Data(node_t) (node_t->data)
#define List_Next(node_t) (node_t->next)
#define List_Prev(node_t) (node_t->prev)

#endif // DATA_H_INCLUDED
