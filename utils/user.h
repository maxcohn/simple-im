// user.h

#ifndef USER_H
#define USER_H

#include <pthread.h>
#include "list.h"


// structs and functions for user_t and lists of users

typedef struct user{
    int socket_id; // descriptor of accepted socket
    int user_id; // numerical ID to track users
    char *username; // username
    pthread_t *thread; // listener thread for user
} user_t;

//===============================================================
// user_t functions
//===============================================================

/**
 * Create a new user
 * 
 * @param socket_id ID of socket being assigned to this user
 * @param user_id Unqiue ID being assigned to this user
 * @param username Username of the user
 * 
 * @return Pointer to user_t struct of this new user
 */
user_t *user_create(int socket_id, int user_id, char *username);

/**
 * Delete a user and all memory associated with it
 * 
 * @param user user_t pointer to be deleted
 */
void user_delete(user_t *user);


//================================================================
// User list functions
//================================================================

/**
 * Adds a new user to the list of current users
 * 
 * @param cur_users List of current users
 * @param new_user User to add to list
 */
void user_login(List *cur_users, user_t *new_user);

/**
 * Removes the given user from the list of current users
 * 
 * @param cur_users List of current users
 * @param user_id ID of user to be removed
 */
void user_logout(List *cur_users, int id);

#endif