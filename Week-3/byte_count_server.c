#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsockfd, n;
    char buffer[100];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8899);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 5);

    printf("Byte Count Server started...\n");
    printf("Waiting for client...\n");

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

    printf("Client connected.\n");

    n = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);

    if (n > 0) {
        buffer[n] = '\0';

        printf("Received Frame: %s\n", buffer);
        printf("Byte Count: %d\n", (unsigned char)buffer[0]);
        printf("Data: %s\n", buffer + 1);
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}