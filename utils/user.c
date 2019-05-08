// user.c
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"
#include "user.h"

user_t *user_create(int socket_id, int user_id, char *username){
    user_t *new_user = malloc(sizeof(user_t));

    new_user->socket_id = socket_id;
    new_user->user_id = user_id;
    new_user->username = NULL;

    new_user->username = calloc(strlen(username) + 1, sizeof(char));
    
    strncpy(new_user->username, username, strlen(username));

    new_user->thread = malloc(sizeof(pthread_t));

    return new_user;
}

void user_delete(user_t *user){
    free(user->username);
    free(user->thread);
    free(user);
}

void user_login(List *cur_users, user_t *new_user){
    list_append(cur_users, (void *) new_user);

    //TODO spawn thread here?
}

void user_logout(List *cur_users, int id){
    //TODO take user_t* instad of int for id???
    for(int i = 0; i < list_size(cur_users); i++){
        user_t *u = (user_t *) list_get(cur_users, i);
        if(u->user_id == id){
            // remove all trace of the user
            list_remove(cur_users, i);
            close(u->socket_id);
            user_delete(u);
            
            return;
        }
    }
}
