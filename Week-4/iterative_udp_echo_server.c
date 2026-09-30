#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd, n;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8899);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    printf("Iterative UDP Echo Server started on port 8899...\n");
    printf("Waiting for client...\n");

    while (1) {
        n = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&cli_addr,
            &cli_len
        );

        if (n < 0) {
            perror("Receive error");
            continue;
        }

        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        sendto(
            sockfd,
            buffer,
            n,
            0,
            (struct sockaddr *)&cli_addr,
            cli_len
        );
    }

    close(sockfd);

    return 0;
}