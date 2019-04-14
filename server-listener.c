// thread spawned by server for each user. Used to listen for new messages


#include <pthread.h>




// thread function
void *server_listener(user_t *user){

    // main listening loop
    while(1){
        //TODO read user input



        //TODO send message to all other users
    }
}