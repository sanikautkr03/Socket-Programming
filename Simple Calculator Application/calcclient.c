#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9002);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        printf("Error connecting to server...\n");
        return -1;
    }

    int num1, num2, result;
    char op;

    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter an operator (+, -, *, /): ");
    scanf(" %c", &op);
    printf("Enter second number: ");
    scanf("%d", &num2);

    send(sockD, &num1, sizeof(num1), 0);
    send(sockD, &num2, sizeof(num2), 0);
    send(sockD, &op, sizeof(op), 0);

    recv(sockD, &result, sizeof(result), 0);
    printf("Result: %d\n", result);

    close(sockD);
    return 0;
}

