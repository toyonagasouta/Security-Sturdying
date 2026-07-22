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
    const char *message = "Hello, socket world!";

    //1. socket() : ソケットの作成
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //接続先アドレスの準備
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // "127.0.0.1"のような文字列をバイナリ形式のIPアドレスに変換
    if (inet_pton(AF_INET, "127.0.0.1" , &server_addr.sin_addr) <= 0){
        perror("inet_pton failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    //2. connect() : サーバへ接続を要求する
    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    //3. send() : メッセージを送信
    send(sock_fd, message, strlen(message), 0);
    printf("sent: %s\n", message);

    //4. recv() : サーバからの応答を受信
    ssize_t bytes_received = recv(sock_fd, buffer, BUF_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv failed");
    } else {
        buffer[bytes_received] = '\0';
        printf("Received back : %s\n", buffer);
    }

    //5. close() : ソケットを閉じる
    close(sock_fd);

    return 0;
}