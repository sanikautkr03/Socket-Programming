#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Function to perform the requested calculation
int calculate(int num1, int num2, char op) {
    switch (op) {
        case '+': return num1 + num2;          // Addition
        case '-': return num1 - num2;          // Subtraction
        case '*': return num1 * num2;          // Multiplication
        case '/': return (num2 != 0) ? (num1 / num2) : 0; // Division with zero-check
        default: return 0;                     // Default case for invalid operator
    }
}

int main() {
    // Create a socket using IPv4 and TCP
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address structure
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;          // Use IPv4
    servAddr.sin_port = htons(9002);        // Set port number in network byte order
    servAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available network interface

    // Bind the socket to the specified address and port
    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

    // Listen for incoming connections, with a maximum queue of 1
    listen(servSockD, 1);

    printf("Calculator Server is running...\n");

    while (1) {
        // Accept a connection from a client
        int clientSocket = accept(servSockD, NULL, NULL);

        int num1, num2, result; // Variables to hold numbers and the result
        char op;                // Variable to hold the operator
        
        // Receive the first number from the client
        recv(clientSocket, &num1, sizeof(num1), 0);

        // Receive the second number from the client
        recv(clientSocket, &num2, sizeof(num2), 0);

        // Receive the operator from the client
        recv(clientSocket, &op, sizeof(op), 0);

        // Log the received data
        printf("Received: %d %c %d\n", num1, op, num2);
        
        // Perform the calculation
        result = calculate(num1, num2, op);

        // Send the result back to the client
        send(clientSocket, &result, sizeof(result), 0);
        printf("Result sent: %d\n", result);

        // Close the client socket after processing the request
        close(clientSocket);
    }

    // Close the server socket
    close(servSockD);
    return 0;
}
