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
    int acked[TOTAL_FRAMES] = {0};
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

    printf("SR Sender started.\n");

    printf("Enter frame number to simulate loss (-1 for none): ");
    scanf("%d", &lost_frame);

    for (int i = 0; i < TOTAL_FRAMES; i += WINDOW_SIZE) {

        int end = i + WINDOW_SIZE;

        if (end > TOTAL_FRAMES)
            end = TOTAL_FRAMES;

        for (int j = i; j < end; j++) {

            if (j == lost_frame) {
                printf("Frame %d lost during transmission.\n", j);
                continue;
            }

            sprintf(buffer, "%d", j);

            printf("Sending Frame %d\n", j);

            send(
                sockfd,
                buffer,
                strlen(buffer) + 1,
                0
            );
        }

        int count = i;

        while (count < end) {
            int n = recv(
                sockfd,
                buffer,
                sizeof(buffer) - 1,
                0
            );

            if (n <= 0)
                break;

            buffer[n] = '\0';

            printf("Received %s\n", buffer);

            int ack;
            sscanf(buffer, "ACK %d", &ack);

            if (ack >= 0 && ack < TOTAL_FRAMES)
                acked[ack] = 1;

            count = 0;

            for (int k = i; k < end; k++) {
                if (acked[k])
                    count++;
            }
        }

        if (lost_frame >= i && lost_frame < end) {
            printf("Retransmitting Frame %d\n", lost_frame);

            sprintf(buffer, "%d", lost_frame);

            send(
                sockfd,
                buffer,
                strlen(buffer) + 1,
                0
            );

            recv(
                sockfd,
                buffer,
                sizeof(buffer) - 1,
                0
            );

            buffer[sizeof(buffer) - 1] = '\0';

            printf("Received %s\n", buffer);

            lost_frame = -1;
        }
    }

    printf("All frames transmitted successfully.\n");

    close(sockfd);

    return 0;
}