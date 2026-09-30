#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    char data[100];
    char frame[200];
    int i, j = 0;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8899);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter data: ");
    fgets(data, sizeof(data), stdin);
    data[strcspn(data, "\n")] = '\0';

    frame[j++] = '$';

    for (i = 0; data[i] != '\0'; i++) {
        if (data[i] == '$' || data[i] == '/') {
            frame[j++] = '/';
        }

        frame[j++] = data[i];
    }

    frame[j++] = '$';
    frame[j] = '\0';

    printf("Stuffed Data: %s\n", frame);

    send(sockfd, frame, j, 0);

    close(sockfd);

    return 0;
}