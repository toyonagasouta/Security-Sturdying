# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>

#define PORT 5000
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // 1. socket() : ソケットの作成
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // アドレス構造体の準備
    memset(&server_addr , 0 , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //　どのインターフェースからでも受け付ける
    server_addr.sin_port = htons(PORT); // ホストバイトオーダー→ネットワークバイトオーダーへ変換


    // 2. bind() : アドレスとポートをソケットに割り当てる
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 3. listen() : 接続待ち状態にする(バックログは1)
    if (listen(server_fd, 1) < 0){
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("listening on port %d\n", PORT);

    //4 accept() :クライアントからの接続を受け入れる
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0){
        perror("accept faild");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected by %s:%d\n" ,
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    
    //5. recv() : クライアントからのデータ受信
    ssize_t bytes_received = recv(client_fd, buffer, BUF_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv failed");
    } else {
        buffer[bytes_received] = '\0'; //文字列として扱う為のnull終端
        printf("Received: %s\n", buffer);

        //. send() : メッセージを送り返す
        send(client_fd, buffer, bytes_received, 0);
    }

    // 7.close() : ソケットを閉じる
    close(client_fd);
    close(server_fd);

    return 0;
}