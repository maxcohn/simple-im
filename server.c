// server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "server.h"
#include "list.h"

#define PORT 5467
#define BUFFER_SIZE 1024

// examplanation of 'struct sockaddr_in':
//https://beej.us/guide/bgnet/html/multi/sockaddr_inman.html


typedef struct user{
    int socket_id; // descriptor of accepted socket
    int user_id; // numerical ID to track users
    char *username; // username
    pthread_t *thread; // listener thread for user
} user_t;

//TODO flesh this out
user_t *user_create(int socket_id, int user_id, char *username);
void user_delete(user_t *user);


//===============================================================
// user_t functions
//===============================================================
user_t *user_create(int socket_id, int user_id, char *username){
    user_t *new_user = malloc(sizeof(user_t));

    new_user->socket_id = socket_id;
    new_user->user_id = user_id;

    new_user->username = malloc(strlen(username) + 1);
    strncpy(new_user->username, username, strlen(username));

    new_user->thread = malloc(sizeof(pthread_t));

    return new_user;
}

void user_delete(user_t *user){
    free(user->username);
    free(user->thread);
    free(user);
}




//TODO figure out if i should move everything related to user and user_list
//TODO into its own file

//================================================================
// User list functions
//================================================================

/**
 * Adds a new user to the list of current users
 * 
 * @param cur_users List of current users
 * @param new_user User to add to list
 */
void user_login(List *cur_users, user_t *new_user){
    list_append(cur_users, (void *) new_user);

    //TODO spawn thread here?
}

/**
 * Removes the given user from the list of current users
 * 
 * @param cur_users List of current users
 * @param user_id ID of user to be removed
 */
void user_logout(List *cur_users, int id){
    for(int i = 0; i < list_size(cur_users); i++){
        user_t *u = (user_t *) list_get(cur_users, i);
        if(u->user_id == id){
            // remove all trace of the user
            list_remove(cur_users, i);
            user_delete(u);
            
            //TODO close thread here
            
            return;
        }
    }
}

//===============================================
// Thread functions
//===============================================

// struct used to pass arguments to threads
struct thread_args{
    user_t *user;
    List *cur_users;
};

void *server_listener(void *args);

/**
 * Resets the buffer 
 * 
 * @param buffer Buffer to be reset
 */
void clear_buffer(char *buffer){
    int buf_size = strlen(buffer) - 1;
    for(; buf_size--; ) buffer[buf_size] = 0;
}

/**
 * Sets up socket connection for the server
 * 
 * @param addr Pointer to socketaddr_in struct to be built
 * @param socket_id Pointer to socket id to be set
 * 
 * @return Error code. Problem if less that 0
 */
int setup_socket(struct sockaddr_in *addr, int *socket_id){
    // create socket and get socket descriptor
    if((*socket_id = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Failed to create socket");
        return -1;
    }

    // setup address struct
    (*addr).sin_family = AF_INET; // ipv4
    (*addr).sin_addr.s_addr = INADDR_ANY; // local host
    (*addr).sin_port = htons(PORT); // convert port to port int to network byte order

    // bind socket to the port
    if(bind(*socket_id, (struct sockaddr *) addr, sizeof(*addr)) < 0){
        perror("Binding failed");
        return -1;
    }
 
    // begin listening (10 is the size of the listening queue)
    if(listen(*socket_id, 10) < 0){
        perror("Listening failed");
        return -1;
    }

    printf("Listening on port %d.\n", PORT);

    return 1;
}

//TODO document later
void send_all_users(List *all_users, user_t *cur_user, char *buffer){
    for(int i = 0; i < list_size(all_users); i++){
        user_t *user = (user_t *) list_get(all_users, i);

        // don't send to the user that created the message
        if(cur_user->user_id == user->user_id)
            continue;
        send(user->socket_id, buffer, strlen(buffer), 0);
    }
}

int main(int argc, char **argv){
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int socket_id = 0;


    // list of all users currenly connected
    List *cur_users = list_create();

    // total number of users connected, used to generate user id
    int total_users = 0;

    // establish main server socket
    if(setup_socket(&address, &socket_id) < 0){
        perror("Socket creation, binding, or listening failed");
        exit(EXIT_FAILURE);
    }
    
    

    // accept new connections loop
    while(list_size(cur_users) < 6){
        // accept new connection
        int new_socket_id = accept(socket_id, (struct sockaddr *) &address, &addrlen);

        // read in new connection's username
        if(recv(new_socket_id, buffer, BUFFER_SIZE, 0) < 0){
            perror("New user connection failed");
            continue;
        }

        // create new user
        user_t *new_user = user_create(new_socket_id, total_users++, buffer);

        // add user to user list
        user_login(cur_users, new_user);

        //TODO spawn new listener thread for user

        // That's some neat code in that it creates a compound literal
        // and passes a reference to it to the thread function.
        // I do this because I can't let the scope of the struct end
        // before the values are used by the thread.
        pthread_create(new_user->thread, NULL, server_listener,
            &((struct thread_args) {new_user, cur_users})) ;
        

        printf("New user: %s (%d)\n", new_user->username, new_user->user_id);

        // spawn a new thread for the user

        // clear the buffer
        clear_buffer(buffer);
        
    }

    /*
    printf("User limit reached:\n");
    for(int i = 0; i < list_size(cur_users); i++){
        user_t *u = list_get(cur_users, i);
        printf("User: %s, ID: %d\n", u->username, u->user_id);
    }
    puts("Removing user with id 2");
    remove_user(cur_users, 2);
    for(int i = 0; i < list_size(cur_users); i++){
        user_t *u = list_get(cur_users, i);
        printf("User: %s, ID: %d\n", u->username, u->user_id);
    }

    */


    return 0;
}








/**
 * Listener thread function for retrieving client info
 * 
 * @param args thread_args pointer that stores argument info
 */
void *server_listener(void *args){

    List *all_users = ((struct thread_args *) args)->cur_users;
    user_t *user = ((struct thread_args *) args)->user;
    char buffer[BUFFER_SIZE] = {0};

    // main listening loop
    while(1){
        // read user input
        if(recv(user->socket_id, buffer, BUFFER_SIZE, 0) < 0){
            perror("Failed to read from user");
            return NULL;
        }

        printf("%s: %s", user->username, buffer);

        // send message to all other users
        send_all_users(all_users, user, buffer);

        clear_buffer(buffer);
        
    }
}