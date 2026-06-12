#include <pthread.h>
#include "request.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "response.h"

void* handlerequest(void* arg){

    client_t data = *(client_t*) arg;
    free((client_t*)arg);
    
    int clientfd = data.clientfd;
    struct sockaddr_in clientaddr = data.clientaddr;

    char clientip[INET_ADDRSTRLEN];
    inet_ntop(clientaddr.sin_family, &clientaddr.sin_addr, clientip, sizeof(clientip));
    
    printf("Connected with %s\n", clientip);
    fflush(stdout);

    //TODO: Recieve the stuff
    char *rdata = retrievedata(clientfd);
    printf("%s", rdata);

    //TODO: send back some stuff
    parserequest(clientfd, rdata);
    
    free(rdata);
    close(clientfd);
    pthread_detach(pthread_self());
    return NULL;
}

char* retrievedata(int fd){
    char *data = NULL;
    char *buff = malloc(DEFAULT_BUFFSIZE);
    int totalbytesrecieved = 0;
    
    while (1)
    {        
        ssize_t recieved_bytes = recv(fd, buff, DEFAULT_BUFFSIZE, 0);
        
        if (recieved_bytes <= 0) {
            break; 
        }

        data = realloc(data, totalbytesrecieved + recieved_bytes + 1);
        memcpy(data + totalbytesrecieved, buff, recieved_bytes);
        
        totalbytesrecieved += recieved_bytes;
        data[totalbytesrecieved] = '\0';

        if(strstr(data, "\r\n\r\n")){
            break;
        }
    }

    free(buff);
    buff=NULL;

    return data;
}