#include <arpa/inet.h>
#include <pthread.h>
#include "request.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void* handlerequest(void* arg){
    client_t data = *(client_t*) arg;
    free(arg);
    
    int clientfd = data.clientfd;
    struct sockaddr_in clientaddr = data.clientaddr;
    char clientip[INET_ADDRSTRLEN];
    inet_ntop(clientaddr.sin_family, &clientaddr.sin_addr, clientip, sizeof(clientip));
    printf("Connected with %s\n", clientip);
    fflush(stdout);

    //TODO: send back some stuff


    close(clientfd);
    pthread_detach(pthread_self());
    return NULL;
}