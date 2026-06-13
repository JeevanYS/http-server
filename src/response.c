#include <sys/socket.h>
#include "request.h"
#include "response.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void parserequest(int clientfd , char *requestdata){
    char *path = parsepath(requestdata);
    if (path == NULL) {
        printf("Path nnot defined???");
        return;
    }
    int pathsize = strlen(path);
    char *formatedpath = malloc(pathsize+13); 


    if(strcmp(path, "/") == 0){
        strcpy(formatedpath, "./index.html");
    }
    else
    if(strchr(path, '.')){
        snprintf(formatedpath, pathsize+2, ".%s", path);
    }
    else{
        snprintf(formatedpath, pathsize+13, ".%s/index.html", path);
    }
    printf("Formated Path: %s\n", formatedpath);
    //send body
    sendfilestream(clientfd, formatedpath);
    
    free(formatedpath);
    free(path);
}

// TODO: well make this thing work
void sendfilestream(int fd, char *formatedpath){

    
    FILE *file;
    file = fopen(formatedpath, "r"); 
    
    if(file){
        char *filetype = strrchr(formatedpath, '.') + 1;
        char responseheader[256];
        int size = sprintf(responseheader, "HTTP/1.1 200 OK\r\nContent-Type: text/%s\r\n\r\n", filetype);
        send(fd, responseheader, size, 0);
        
        
        fseek(file, 0, SEEK_END);
        int filesize = ftell(file);
        rewind(file);
        char *filecontent = malloc(filesize);
    
        fread(filecontent, filesize, 1, file);
        send(fd, filecontent, filesize, 0);

        free(filecontent);
        fclose(file);
    }
    else{
        char code[] = "HTTP/1.1 404\r\nContent-Type: text/html\r\n\r\n <h1>404 Page Not Found!!!</h1>";
        send(fd, code, sizeof(code), 0);
    }
    
}

char* parsepath(char *requestdata){
    char *path = NULL;

    char *startingpath = NULL;
    char *endingpath = NULL;
    
    printf("Entered the parsepath function!!!");
    
    if(strncmp(requestdata, "GET", 3) == 0){
        startingpath = requestdata + 4;
        endingpath = strchr(startingpath, ' ');

        int pathlength = endingpath-startingpath;
        printf("Recieved a path with length of : %d\n", pathlength);
        
        path = malloc(pathlength+1);
        memcpy(path, startingpath, pathlength);
        path[pathlength] = '\0';
        printf("The path is: %s", path);
    }
    fflush(stdout);
    return path;
}