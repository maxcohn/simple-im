// testing list data type


#include "list.h"
#include <stdio.h>


int main(){

    printf("Begin list testing\n");

    List *l = list_create();
    list_append(l, (void *)15 );
    list_append(l, (void *)35 );
    for(int i = 0; i < 10; i++){
        //list_append(l, (void *)i);
    }
    
    for(int i = 0; i < list_size(l); i++){
        //printf("%d ", (int)list_get(l, i));
    }

    printf("current size: %d\n", list_size(l));

    puts("");

    list_remove(l, 3);

    for(int i = 0; i < list_size(l); i++){
        //printf("%d ", (int)list_get(l, i));
    }
    printf("current size: %d\n", list_size(l));

    puts("");

    list_delete(l);
    
    



}