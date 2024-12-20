#include <netinet/in.h>  // structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // for socket APIs
#include <sys/types.h>
#include <unistd.h>      // for close()

int main(int argc, char const* argv[]) {
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int id1, id2, val, otp, serverResponse;

    printf("Enter Your Account ID: ");
    scanf("%d", &id1);

    printf("Enter Account ID to send money: ");
    scanf("%d", &id2);

    printf("Enter Amount: ");
    scanf("%d", &val);

    if (connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        printf("Error connecting to server...\n");
        return -1;
    }

    // Receive OTP from server
    recv(sockD, &otp, sizeof(otp), 0);
    printf("OTP received: %d\n", otp);

    // Enter OTP to send back to server
    printf("Enter the OTP sent to you: ");
    int clientOtp;
    scanf("%d", &clientOtp);
    send(sockD, &clientOtp, sizeof(clientOtp), 0);

    // Check OTP validation
    recv(sockD, &serverResponse, sizeof(serverResponse), 0);
    if (serverResponse == -1) {
        printf("Invalid OTP. Exiting.\n");
        close(sockD);
        return -1;
    }
    printf("OTP Verified.\n");

    // Debit Transaction
    int acti = 0;  // Debit action
    send(sockD, &acti, sizeof(acti), 0);
    send(sockD, &id1, sizeof(id1), 0);
    send(sockD, &val, sizeof(val), 0);

    recv(sockD, &serverResponse, sizeof(serverResponse), 0);
    if (serverResponse == 9999) {
        printf("Insufficient Funds\n");
        close(sockD);
        return -1;
    } else {
        printf("Debit Successful\n");
    }

    // Credit Transaction
    acti = 1;  // Credit action
    send(sockD, &acti, sizeof(acti), 0);
    send(sockD, &id2, sizeof(id2), 0);
    send(sockD, &val, sizeof(val), 0);

    recv(sockD, &serverResponse, sizeof(serverResponse), 0);
    if (serverResponse == 1) {
        printf("Credit Successful\n");
    } else {
        printf("Error in Credit Transaction\n");
    }

    close(sockD);
    return 0;
}

