#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// Main function to handle the client-side of the application
int main() {
    // Create a socket using IPv4 and TCP
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address structure
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;        // Use IPv4
    servAddr.sin_port = htons(9002);      // Set port number in network byte order
    servAddr.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface

    // Attempt to connect to the server
    if (connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        printf("Error connecting to server...\n"); // Print error if connection fails
        return -1;
    }

    int num1, num2, result; // Variables to hold numbers and the result
    char op;                // Variable to hold the operator

    // Input the first number
    printf("Enter first number: ");
    scanf("%d", &num1);

    // Input the operator
    printf("Enter an operator (+, -, *, /): ");
    scanf(" %c", &op);

    // Input the second number
    printf("Enter second number: ");
    scanf("%d", &num2);

    // Send the first number to the server
    send(sockD, &num1, sizeof(num1), 0);

    // Send the second number to the server
    send(sockD, &num2, sizeof(num2), 0);

    // Send the operator to the server
    send(sockD, &op, sizeof(op), 0);

    // Receive the result of the operation from the server
    recv(sockD, &result, sizeof(result), 0);

    // Print the result
    printf("Result: %d\n", result);

    // Close the socket connection
    close(sockD);
    return 0;
}
