#ifndef REQUEST_H
#define REQUEST_H
#include <arpa/inet.h>

typedef struct {
    int clientfd;
    struct sockaddr_in clientaddr;
} client_t;

void* handlerequest(void* arg);

#endif