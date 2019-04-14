#include <netinet/in.h>

/**
 * Sets up the main server socket
 * 
 * @param addr Pointer to socketaddr_in struct
 * @param socket_id Pointer socket's file descriptor
 * @return Error status. Less than zero reports an error
 */
int setup_socket(struct sockaddr_in *addr, int *socket_id);