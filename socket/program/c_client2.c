#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUF_SIZE 1024

int main(){
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char input[BUF_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1" , &server_addr.sin_addr) <= 0) {
        perror("inet_ptons failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    //ループ
    while (1) {
        printf("Enter message (or 'quit' to exit): ");
        fflush(stdout); //プロンプトを確実に表示

        if(fgets(input, BUF_SIZE, stdin) == NULL ) {
            break;
        }

        // fgetsは末尾に改行を含むので取り除く
        input[strcspn(input, "\n")] = '\0';

        //空文字列チェック
        if (strlen(input) == 0){
            printf("Empty message, please enter something. \n");
            continue;
        }

        send(sock_fd, input, strlen(input), 0);

        if (strcmp(input, "quit") == 0) {
            break;
        }

        ssize_t bytes_received = recv(sock_fd, buffer, BUF_SIZE, 0);
        if (bytes_received <= 0) {
            printf("server closed connection.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received back: %s\n", buffer);
    }

    close(sock_fd);

    return 0;
}