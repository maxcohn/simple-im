// client.c


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#include "client.h"

#define PORT 5467
#define BUFFER_SIZE 1024

// return value of the listener thread. used to tell if connection was lost
static int thread_ret = 1;

void print_usage(void){
    printf("Usage: chat-client ip username\n");
}

/**
 * Resets the buffer 
 * 
 * @param buffer Buffer to be reset
 */
void clear_buffer(char *buffer){
    int buf_size = strlen(buffer) - 1;
    for(; buf_size--; ) buffer[buf_size] = 0;
}

void *client_listener(void *socket){
    int socket_id = (int) socket;
    char buffer[BUFFER_SIZE];

    // main listening loop
    while(1){
        // read data from socket
        thread_ret = recv(socket_id, buffer, BUFFER_SIZE, 0);
        
        // if the connect is closed, recv() returns 0
        if(thread_ret == 0){
            // close socket and thread
            close(socket_id);
            pthread_exit(&thread_ret);
        }else if(thread_ret < 0){
            perror("Failed to read from user");
            return NULL;
        }

        printf("%s", buffer);

        clear_buffer(buffer);
    }

}

int main(int argc, char** argv){
    // argv[0] = chat-client
    // argv[1] = ip
    // argv[2] = username

    // check arguemnts
    if(argc != 3){
        print_usage();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    pthread_t listener_thread;

    char username[strlen(argv[2]) + 2];

    strncpy(username, argv[2], strlen(argv[2]));

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_id < 0){
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }

    // fill server_addr with '0'
    memset(&server_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0){
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    int user_con = connect(socket_id, (struct sockaddr *) &server_addr, sizeof(server_addr));

    if(user_con < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // create listening thread
    pthread_create(&listener_thread, NULL, client_listener,
        (void *) socket_id);

    // send username to server socket
    send(socket_id, username, strlen(username), 0);

    // main loop
    while(1){
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            return -1;

        // check if the listening thread ended due to lost connection
        if(thread_ret == 0){
            printf("Connection to server lost.\n");
            exit(1);
        }

        send(socket_id, buffer, strlen(buffer), 0);

        clear_buffer(buffer);
    }

    return 0;
}