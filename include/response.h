#ifndef RESPONSE_H
#define RESPONSE_H

void parserequest(int clientfd ,char *requestdata);
char* parsepath(char *requestdata);
void sendfilestream(int fd, char *path);

#endif