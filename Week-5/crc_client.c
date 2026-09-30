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
    int sockfd;
    char data[100];
    char generator[50];
    char appended[200];
    char temp[200];
    char crc[50];
    char result[200];

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

    printf("Enter data bits: ");
    scanf("%s", data);

    printf("Enter generator: ");
    scanf("%s", generator);

    int data_len = strlen(data);
    int gen_len = strlen(generator);
    int crc_len = gen_len - 1;

    strcpy(appended, data);

    for (int i = 0; i < crc_len; i++)
        strcat(appended, "0");

    strcpy(temp, appended);

    divide(temp, generator, strlen(appended), gen_len);

    strcpy(crc, temp + data_len);

    strcpy(appended, data);
    strcat(appended, crc);

    printf("CRC: %s\n", crc);
    printf("Codeword: %s\n", appended);

    send(sockfd, appended, strlen(appended) + 1, 0);
    send(sockfd, generator, strlen(generator) + 1, 0);

    recv(sockfd, result, sizeof(result), 0);

    printf("Server: %s\n", result);

    close(sockfd);

    return 0;
}