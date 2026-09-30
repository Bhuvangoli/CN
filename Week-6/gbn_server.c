#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsockfd;
    int frame, expected = 0;
    char buffer[100];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

    listen(sockfd, 5);

    printf("GBN Receiver started on port 8899...\n");
    printf("Waiting for sender...\n");

    newsockfd = accept(
        sockfd,
        (struct sockaddr *)&cli_addr,
        &cli_len
    );

    printf("Sender connected.\n");

    while (1) {
        int n = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        frame = atoi(buffer);

        if (frame == expected) {
            printf("Received Frame %d\n", frame);

            sprintf(buffer, "ACK %d", frame);
            send(newsockfd, buffer, strlen(buffer) + 1, 0);

            expected++;
        } else {
            printf("Frame %d discarded. Expected Frame %d\n",
                   frame, expected);

            sprintf(buffer, "ACK %d", expected - 1);
            send(newsockfd, buffer, strlen(buffer) + 1, 0);
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}