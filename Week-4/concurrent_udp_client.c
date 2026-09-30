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
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8899);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0)
            break;

        sendto(
            sockfd,
            buffer,
            strlen(buffer),
            0,
            (struct sockaddr *)&serv_addr,
            serv_len
        );

        n = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&serv_addr,
            &serv_len
        );

        buffer[n] = '\0';

        printf("Server: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}