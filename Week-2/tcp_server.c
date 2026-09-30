#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsockfd, n;
    socklen_t cli_addr_len;
    char buffer[80];
    struct sockaddr_in cli_addr, serv_addr;

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

    printf("Server started on port 8899...\n");
    printf("Waiting for client connection...\n");

    for (int i = 0; i < 5; i++) {
        cli_addr_len = sizeof(cli_addr);

        newsockfd = accept(
            sockfd,
            (struct sockaddr *)&cli_addr,
            &cli_addr_len
        );

        if (newsockfd < 0) {
            perror("Accept error");
            exit(1);
        }

        printf("Client connected\n");

        for (int j = 0; j < 5; j++) {
            n = recv(newsockfd, buffer, 80, 0);

            if (n <= 0)
                break;

            printf("Server received: ");
            fflush(stdout);

            write(1, buffer, n);

            send(newsockfd, buffer, n, 0);
        }

        close(newsockfd);
        printf("\nClient disconnected.\n");
    }

    close(sockfd);

    return 0;
}