#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    char data[200];
    char stuffed[300];
    int i, j = 0, count = 0;

    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8899);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter binary data: ");
    scanf("%199s", data);

    for (i = 0; data[i] != '\0'; i++) {
        stuffed[j++] = data[i];

        if (data[i] == '1') {
            count++;

            if (count == 5) {
                stuffed[j++] = '0';
                count = 0;
            }
        } else {
            count = 0;
        }
    }

    stuffed[j] = '\0';

    printf("Stuffed Data: %s\n", stuffed);

    send(sockfd, stuffed, strlen(stuffed), 0);

    close(sockfd);

    return 0;
}