#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFLEN 256  // Buffer length for message communication
#define MAX 80      // Maximum buffer size for user input and server response

// Function to handle communication with the server
void communicate_with_server(int sockfd) {
    char buff[MAX];
    for (;;) {
        // Clear the buffer for user input
        memset(buff, 0, sizeof(buff));
        printf("Enter the string: ");
        
        // Read input from the user
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("Input error"); // Handle input error
            break;
        }

        // Exit condition when the user inputs "exit"
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // Send the user input to the server
        if (write(sockfd, buff, strlen(buff)) < 0) {
            perror("Write to server failed"); // Handle write error
            break;
        }

        // Clear the buffer to receive the server response
        memset(buff, 0, sizeof(buff));
        int bytes_read = read(sockfd, buff, sizeof(buff) - 1); // Read response from server
        if (bytes_read < 0) {
            perror("Read from server failed"); // Handle read error
            break;
        }
        buff[bytes_read] = '\0';  // Null-terminate the received string
        printf("From Server: %s", buff); // Display the server response
    }
}

int main(int argc, char **argv) {
    // Check if the port number is provided as a command-line argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]); // Display usage instructions
        exit(EXIT_FAILURE);
    }

    int sd, port;
    struct sockaddr_in server;

    // Convert the port argument from string to integer
    port = atoi(argv[1]);

    // Create a socket for communication
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed"); // Handle socket creation failure
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;             // Use IPv4
    server.sin_port = htons(port);           // Convert port to network byte order
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set server address to localhost

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection to server failed"); // Handle connection failure
        close(sd); // Close the socket
        exit(EXIT_FAILURE);
    }
  
    // Start communication with the server
    communicate_with_server(sd);  
    
    // Close the socket after communication ends
    close(sd); 
    return 0;
}
