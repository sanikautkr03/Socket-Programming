#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define SERVER_TCP_PORT 5750
#define BUFLEN 256           
#define MAX 80              

void handle_client(int sockfd) {
    char buff[MAX];
    int n;

    for (;;) {
        memset(buff, 0, MAX);
        
        int read_bytes = read(sockfd, buff, sizeof(buff) - 1);
        if (read_bytes < 0) {
            perror("Read from client failed");
            break;
        }
        buff[read_bytes] = '\0'; 
        printf("From client: %s\nTo client: ", buff);

        memset(buff, 0, MAX);
        if (fgets(buff, MAX, stdin) == NULL) {
            perror("Input failed");
            break;
        }

        if (write(sockfd, buff, strlen(buff)) < 0) {
            perror("Write to client failed");
            break;
        }

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main(int argc, char **argv) {
    int sd, new_sd, port;
    struct sockaddr_in server, client;
    socklen_t client_len;
    int yes = 1;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("Setting socket options failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);          
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding socket failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    if (listen(sd, 5) == -1) {
        perror("Listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    while (1) {
        client_len = sizeof(client);
        if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
            perror("Accept failed");
            continue; 
        }

        printf("Connected to client\n");
        handle_client(new_sd); 
        close(new_sd);        
        printf("Client disconnected\n");
    }

    close(sd);
    return 0;
}

