#include <sys/socket.h>
#include "request.h"
#include "response.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void parserequest(int clientfd , char *requestdata){
    //send request ok + header
    


    //send body
    sendfilestream(clientfd, parsepath(requestdata));

}

// TODO: well make this thing work
void sendfilestream(int fd, char *path){
    char formatedpath[strlen(path)+13]; 
    if(strcmp(path, "/") == 0){
        strcpy(formatedpath, "./index.html");
    }
    else{
        snprintf(formatedpath, sizeof(formatedpath), ".%s/index.html", path);
    }
    printf("Formated Path: %s\n", formatedpath);
    
    FILE *file;
    file = fopen(formatedpath, "r"); 
    
    if(file){
        char code[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(fd, code, sizeof(code), 0);
        
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
        char code[] = "HTTP/1.1 404\r\n\r\n <h1>404 Page Not Found!!!</h1>";
        send(fd, code, sizeof(code), 0);
    }
    
    free(path);
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