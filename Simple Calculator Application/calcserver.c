#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int calculate(int num1, int num2, char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? (num1 / num2) : 0; // Avoid division by zero
        default: return 0;
    }
}

int main() {
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9002);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(servSockD, 1);

    printf("Calculator Server is running...\n");

    while (1) {
        int clientSocket = accept(servSockD, NULL, NULL);

        int num1, num2, result;
        char op;
        
        recv(clientSocket, &num1, sizeof(num1), 0);
        recv(clientSocket, &num2, sizeof(num2), 0);
        recv(clientSocket, &op, sizeof(op), 0);

        printf("Received: %d %c %d\n", num1, op, num2);
        
        result = calculate(num1, num2, op);

        send(clientSocket, &result, sizeof(result), 0);
        printf("Result sent: %d\n", result);

        close(clientSocket);
    }

    close(servSockD);
    return 0;
}

