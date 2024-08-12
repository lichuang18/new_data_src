#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5252
#define STORAGE_IP "10.0.50.4"
int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *sth = "FC"; // 要传递的字符串
    char buffer[1024] = {0};

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IP地址转换为二进制
    if(inet_pton(AF_INET, STORAGE_IP , &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    //
    while(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
	    printf("No Target recv Data\r");
    }	

    printf("\nStart Trans Data\n");




    // 发送字符串
    sth = argv[1];
    send(sock, sth, strlen(sth), 0);
    printf("String sent\n");

    // 等待确认消息
    read(sock, buffer, 1024);
    if (strcmp(buffer, "received") == 0) {
        printf("成功\n");
    }

    close(sock);
    return 0;
}

