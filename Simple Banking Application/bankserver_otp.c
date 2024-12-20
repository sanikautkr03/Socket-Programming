#include <netinet/in.h>  // structure for storing address information
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  // for socket APIs
#include <sys/types.h>
#include <unistd.h>      // for close()
#include <time.h>        // for random OTP generation

int main(int argc, char const* argv[]) {
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);

    // Define server address
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int acc_arr[] = {100, 200, 500, 700, 800};  // Account balances
    int acti, accno, amt, otp, clientOtp;

    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(servSockD, 1);

    while (1) {
        printf("ano\tamt\n");
        for (int i = 0; i < 5; i++) {
            printf("%d\t%d\n", i, acc_arr[i]);
        }

        int clientSocket = accept(servSockD, NULL, NULL);

        // Generate and send OTP
        srand(time(NULL));
        otp = rand() % 10000;  // Generate a 4-digit OTP
        printf("Generated OTP: %d\n", otp);
        send(clientSocket, &otp, sizeof(otp), 0);

        // Receive OTP from client
        recv(clientSocket, &clientOtp, sizeof(clientOtp), 0);

        if (clientOtp != otp) {
            printf("Invalid OTP received.\n");
            int response = -1;
            send(clientSocket, &response, sizeof(response), 0);
            close(clientSocket);
            continue;
        } else {
            printf("OTP Verified.\n");
            int response = 1;
            send(clientSocket, &response, sizeof(response), 0);
        }

        // Debit Transaction
        recv(clientSocket, &acti, sizeof(acti), 0);  // Action type
        recv(clientSocket, &accno, sizeof(accno), 0);  // Source account
        recv(clientSocket, &amt, sizeof(amt), 0);  // Amount to debit

        if (acti == 0) {  // Debit
            if (amt > acc_arr[accno]) {
                int msg = 9999;  // Insufficient funds
                send(clientSocket, &msg, sizeof(msg), 0);
            } else {
                acc_arr[accno] -= amt;  // Deduct amount
                int msg = 1;  // Success
                send(clientSocket, &msg, sizeof(msg), 0);
            }
        }

        // Credit Transaction
        recv(clientSocket, &acti, sizeof(acti), 0);  // Action type
        recv(clientSocket, &accno, sizeof(accno), 0);  // Destination account
        recv(clientSocket, &amt, sizeof(amt), 0);  // Amount to credit

        if (acti == 1) {  // Credit
            acc_arr[accno] += amt;  // Add amount
            int msg = 1;  // Success
            send(clientSocket, &msg, sizeof(msg), 0);
        }

        close(clientSocket);
    }

    return 0;
}

