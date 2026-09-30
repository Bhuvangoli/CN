#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void divide(char data[], char generator[], int data_len, int gen_len) {
    int i, j;

    for (i = 0; i <= data_len - gen_len; i++) {
        if (data[i] == '1') {
            for (j = 0; j < gen_len; j++) {
                data[i + j] =
                    (data[i + j] == generator[j]) ? '0' : '1';
            }
        }
    }
}

int main() {
    int sockfd, newsockfd;
    int n;
    char data[200];
    char generator[50];
    char result[200];

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

    printf("CRC Server started on port 8899...\n");
    printf("Waiting for client...\n");

    newsockfd = accept(
        sockfd,
        (struct sockaddr *)&cli_addr,
        &cli_len
    );

    if (newsockfd < 0) {
        perror("Accept error");
        exit(1);
    }

    printf("Client connected.\n");

    recv(newsockfd, data, sizeof(data), 0);
    recv(newsockfd, generator, sizeof(generator), 0);

    printf("Received Codeword: %s\n", data);
    printf("Generator: %s\n", generator);

    int data_len = strlen(data);
    int gen_len = strlen(generator);

    char temp[200];

    strcpy(temp, data);

    divide(temp, generator, data_len, gen_len);

    int error = 0;

    for (int i = data_len - gen_len + 1; i < data_len; i++) {
        if (temp[i] == '1') {
            error = 1;
            break;
        }
    }

    if (error)
        strcpy(result, "CRC Error Detected");
    else
        strcpy(result, "No CRC Error - Data Received Correctly");

    printf("%s\n", result);

    send(newsockfd, result, strlen(result) + 1, 0);

    close(newsockfd);
    close(sockfd);

    return 0;
}