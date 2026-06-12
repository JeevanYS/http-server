#ifndef REQUEST_H
#define REQUEST_H
#include <arpa/inet.h>

#define RECV_BUFFSIZE 4096

typedef struct {
    int clientfd;
    struct sockaddr_in clientaddr;
} client_t;

void* handlerequest(void* arg);

char* retrievedata(int fd);

#endif