#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUF_SIZE 1024

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr ,client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // SO_REUSEADDR: サーバ再起動時の""Address already in use" 対策
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected by %s:%d\n",
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    
    // ループ化
    while (1) {
        ssize_t bytes_received = recv(client_fd, buffer, BUF_SIZE, 0);

        if(bytes_received < 0){
            perror("recv failed");
            break;
        }
        if(bytes_received == 0) {
            //クライアントがclose()した合図(Pythonの'not data'に相当)
            printf("Client disconnected");
            break;
        }

        buffer[bytes_received] = '\0';

        if(strcmp(buffer, "quit") == 0) {
            printf("Client requested to quit. \n");
            break;
        }

        printf("Received: %s\n", buffer);
        send(client_fd, buffer, bytes_received, 0);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}