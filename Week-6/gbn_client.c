#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TOTAL_FRAMES 10
#define WINDOW_SIZE 4

int main() {
    int sockfd;
    int base = 0;
    int next_frame = 0;
    int lost_frame;
    char buffer[100];
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8899);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(
        sockfd,
        (struct sockaddr *)&serv_addr,
        sizeof(serv_addr)) < 0) {

        perror("Connection error");
        exit(1);
    }

    printf("GBN Sender started.\n");

    printf("Enter frame number to simulate loss (-1 for none): ");
    scanf("%d", &lost_frame);

    while (base < TOTAL_FRAMES) {

        while (next_frame < base + WINDOW_SIZE &&
               next_frame < TOTAL_FRAMES) {

            if (next_frame == lost_frame) {
                printf("Frame %d lost during transmission.\n",
                       next_frame);
            } else {
                sprintf(buffer, "%d", next_frame);

                printf("Sending Frame %d\n", next_frame);

                send(
                    sockfd,
                    buffer,
                    strlen(buffer) + 1,
                    0
                );
            }

            next_frame++;
        }

        int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        printf("Received %s\n", buffer);

        int ack;
        sscanf(buffer, "ACK %d", &ack);

        if (ack >= base) {
            base = ack + 1;
        }

        if (lost_frame >= base) {
            next_frame = base;
            lost_frame = -1;
        }
    }

    printf("All frames transmitted successfully.\n");

    close(sockfd);

    return 0;
}