#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int sendhtmlasplaintext(char *filename, int socket_fd);

int main(void)
{
    int server_fd;
    struct addrinfo hints, *res;
    int status;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Bind to local machine

    status = getaddrinfo(NULL, "10010", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        freeaddrinfo(res);
        return 0;
    }

    freeaddrinfo(res);

    if (listen(server_fd, 20) == -1) {
        perror("listen");
        return 1;
    }

    printf("Listening on port 10010...\n");

    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(
            server_fd,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        if (client_fd == -1) {
            perror("accept");
            continue;
        }
        
        printf("Client connected!\n");

        int max_len = 1000;
        char http_request[max_len];
        memset(http_request, 0, max_len);

        int bytes_received = recv(client_fd, http_request, max_len, 0);

        if (strncmp(http_request, "GET", 3) == 0){
            printf("yepp you got GET request...");
            
            char *status_line = "HTTP/1.1 200 OK\r\n";
            char *response_body = "<h1>JEEVAN!!!!!</h1>\r\n";
            send(client_fd, status_line, strlen(status_line), 0);
            char *headers = "Content-Type: text/html\r\n"
                            "\r\n";
            send(client_fd, headers, strlen(headers), 0);
            printf("Cliend File Descriptor: %d", client_fd);
            sendhtmlasplaintext("index.html", client_fd);

            
        }
        close(client_fd);     

    }

    close(server_fd);
    return 0;
}

int sendhtmlasplaintext(char *filename, int socket_fd){
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    char *content = malloc(file_size);
    fread(content, 1, file_size, file);
    send(socket_fd, content, strlen(content),0);

    printf("File Size: %d", file_size);
    fclose(file);

}