#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "request.h"
#include <pthread.h>
#include <string.h>

int main(){
    int sockfd, clientfd; 
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("Socket: ");
        return 1;
    }
    
    struct sockaddr_in addr, clientaddr;
    memset(&addr, 0, sizeof(addr));
    
    socklen_t client_len = sizeof(clientaddr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    int bindres = bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));

    if (bindres < 0){
        perror("Bind");
        return 1;
    }

    if(listen(sockfd, 20)){
        perror("Listen");
        return 1;
    }
    
    while (1){
        clientfd = accept(sockfd, (struct sockaddr*) &clientaddr, &client_len);
        if(clientfd < 0){
            perror("Accept");
        }

        client_t *dataofclient = malloc(sizeof(client_t));
        dataofclient->clientfd = clientfd;
        dataofclient->clientaddr = clientaddr;

        pthread_t thread;
        pthread_create(&thread, NULL, handlerequest, (void*)dataofclient);
    }
}