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

#define SERVER_TCP_PORT 5750 // Default server port
#define BUFLEN 256           // Buffer length for communication
#define MAX 80               // Maximum buffer size for messages

// Function to handle communication with a connected client
void handle_client(int sockfd) {
    char buff[MAX];
    int n;

    for (;;) {
        // Clear the buffer for reading data
        memset(buff, 0, MAX);
        
        // Read data from the client
        int read_bytes = read(sockfd, buff, sizeof(buff) - 1);
        if (read_bytes < 0) {
            perror("Read from client failed"); // Handle read failure
            break;
        }
        buff[read_bytes] = '\0';  // Null-terminate the received data
        printf("From client: %s\nTo client: ", buff);

        // Clear the buffer for sending response
        memset(buff, 0, MAX);
        // Read the response from server input
        if (fgets(buff, MAX, stdin) == NULL) {
            perror("Input failed"); // Handle input failure
            break;
        }

        // Send the response to the client
        if (write(sockfd, buff, strlen(buff)) < 0) {
            perror("Write to client failed"); // Handle write failure
            break;
        }

        // Exit condition if "exit" is received
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

    // Check if port number is provided as a command-line argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]); // Display usage instructions
        exit(EXIT_FAILURE);
    }

    // Convert the provided port number from string to integer
    port = atoi(argv[1]);

    // Create a socket for server communication
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed"); // Handle socket creation failure
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow reuse of local addresses
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("Setting socket options failed"); // Handle socket option failure
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Initialize the server address structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;            // Use IPv4
    server.sin_port = htons(port);          // Convert port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set server address to localhost

    // Bind the socket to the specified address and port
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Binding socket failed"); // Handle binding failure
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Start listening for client connections
    if (listen(sd, 5) == -1) {
        perror("Listen failed"); // Handle listen failure
        close(sd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    // Main loop to accept and handle client connections
    while (1) {
        client_len = sizeof(client);
        // Accept a connection from a client
        if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
            perror("Accept failed"); // Handle accept failure
            continue; // Continue to the next iteration for new connections
        }

        printf("Connected to client\n");
        // Handle communication with the connected client
        handle_client(new_sd); 
        close(new_sd);        // Close the client socket
        printf("Client disconnected\n");
    }

    // Close the server socket
    close(sd);
    return 0;
}
