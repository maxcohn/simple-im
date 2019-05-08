#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static struct node *node_create(void* data, struct node *next){
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data = data;
    new_node->next = next;
    
    return new_node;
}

static void node_delete(struct node *n){
    free(n);
}

List *list_create(void){
    List *new_list = malloc(sizeof(List));
    
    new_list->head = NULL;
    new_list->size = 0;
    
    return new_list;
}

void list_delete(List *list){

    // remove all nodes from list
    struct node *cur_node = list->head;
    while(cur_node != NULL){
        struct node *new_node = cur_node->next;
        node_delete(cur_node);
        cur_node = new_node;
    }

    free(list);
}

void list_append(List *list, void *data){
    //DEBUG printf("%d\n", list_empty(list));
    if(list_empty(list)){
        list->head = node_create(data, NULL);
        list->size++;
        return;
    }
    
    struct node *prev_node = NULL;
    struct node *cur_node = list->head;

    while(cur_node->next != NULL){
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
    cur_node->next = node_create(data, NULL);

    list->size++;
}

void *list_get(List *list, int index){
    // if index is greater than last node, return NULL
    if(index >= list_size(list)){
        return NULL;
    }

    struct node *cur_node = list->head;
    for(int i = 0; i < index; i++){
        cur_node = cur_node->next;
    }

    return cur_node->data;
}

void list_remove(List *list, int index){
    // if index is greater than last node, return NULL
    if(index >= list_size(list)){
        return;
    }

    // if we're removing the head of the list
    if(index == 0){
        struct node *old_head = list->head;
        list->head = old_head->next;
        node_delete(old_head);
        list->size--;
        return;
    }

    struct node *prev_node = NULL;
    struct node *cur_node = list->head;

    for(int i = 0; i < index; i++){
        prev_node = cur_node;
        cur_node = cur_node->next;
    }

    prev_node->next = cur_node->next;
    node_delete(cur_node);

    list->size--;

}

int list_size(List *list){
    return list->size;
}

int list_empty(List *list){
    return !list->size;
}