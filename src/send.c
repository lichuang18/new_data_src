#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

#define DEBUG 1
#define PORT 5252
#define STORAGE_IP "10.0.50.4"
int main(int argc, char *argv[]) {
    
    if (argc != 3 || (strcmp(argv[1], "FC") != 0 && strcmp(argv[1], "10Gb") != 0 && strcmp(argv[1], "SRIO") != 0)) {
        printf("Usage: %s <Port> <Record Time>\n", argv[0]);
        return 1;
    }
	
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *sth = "10Gb"; // 要传递的字符串变量
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
    if(DEBUG) printf("String sent\n");

    struct timeval start, end;
    
    
    gettimeofday(&start, NULL);
    int time2 = atoi(argv[2]);
    sleep(time2);
    gettimeofday(&end, NULL);

    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    if(DEBUG) printf("data_source device port[%s] send time: %f s\n", argv[1], execution_time);


    // 等待确认消息
    read(sock, buffer, 1024);
    if (strcmp(buffer, "received") == 0) {
        if(DEBUG) printf("成功\n");
    }

    close(sock);
    return 0;
}

