// List data type
//

struct node{
    void *data;
    struct node *next;
};

typedef struct _list{
    struct node *head;
    int size;

} List;


/**
 * Create a new node
 * 
 * @param data Void pointer to data
 * @param next The next node in the list
 * 
 * @return Pointer to struct node pointer
 */
static struct node *node_create(void* data, struct node *next);

/**
 * Frees all memory associated with node
 * 
 * @param n Node to delte
 */
static void node_delete(struct node* n);

List *list_create(void);

void list_delete(List *list);

void list_append(List *list, void *data);

void *list_get(List *list, int index);

void list_remove(List *list, int index);

int list_size(List *list);

int list_empty(List *list);

