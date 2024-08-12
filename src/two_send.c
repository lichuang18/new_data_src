#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


#define PORT 5252
#define STORAGE_IP1 "10.0.50.4"
#define STORAGE_IP2 "10.0.50.14"
#define DEBUG 1

// 封装线程参数的结构体
typedef struct {
    const char *ip;
    char *param1;
    char *param2;
} ThreadArgs;


void *connect_to_server(void *args) {

    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
	return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, threadArgs->ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
	return NULL;
    }

    struct timeval start, end, end1;
    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    while(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 && execution_time < 10){
        gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("No Target recv Data %s , %fs\r",threadArgs->param2,execution_time);	
    }
    //printf("\n");
    if(execution_time >= 10){
	 printf("\n%s_dev recv timeout\n",threadArgs->param2);
	 return NULL;
    }
    // 发送字符串
    if(threadArgs->ip == STORAGE_IP1){
       printf("\nStart A_DEV Trans Data\n");
    }else if(threadArgs->ip == STORAGE_IP2){
       printf("\nStart B_DEV Trans Data\n");
    }

    const char *sth = "10Gb"; // 要传递的字符串
    char buffer[1024] = {0};
    int sockB, sockC;

    sth = threadArgs->param1;
    send(sock, sth, strlen(sth), 0);
    printf("\nString sent to %s_dev\n", threadArgs->param2);

    // 等待确认消息
    read(sock, buffer, 1024);
    if (strcmp(buffer, "received") == 0) {
        printf("\n%s_dev 成功\n", threadArgs->param2);
    }

    close(sock);

}

int main(int argc, char *argv[]) {

    if (argc != 4 || (strcmp(argv[1], "FC") != 0 && strcmp(argv[1], "10Gb") != 0 && strcmp(argv[1], "SRIO") != 0)) {
        printf("Usage: %s <Port> <Signal> <Record Time>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "LD") != 0 && strcmp(argv[2], "SAR") != 0 && strcmp(argv[2], "IR") != 0 && strcmp(argv[2], "RS") != 0 && strcmp(argv[2], "SPEECH") != 0 && strcmp(argv[2], "multi") != 0) {
        printf("Signal only <LD/SAR/IR/RS/SPEECH/multi>\n");
        return 1;
    }
    pthread_t threadB, threadA;
    //struct ThreadArgs argsB, argsC;
    //
    ThreadArgs argsA = {"ip2", "10Gb", "A"}; // 参数1和参数2是示例数据
    ThreadArgs argsB = {"ip3", "10Gb", "B"};
    
    argsA.ip = STORAGE_IP1;
    argsA.param1 = argv[1];
    argsA.param2 = "A";

    argsB.ip = STORAGE_IP2;
    argsB.param1 = argv[1];
    argsB.param2 = "B";
    // 启动连接到B机器的线程
    if(pthread_create(&threadA, NULL, connect_to_server, &argsA) != 0) {
        perror("Failed to create thread for B");
        return -1;
    }

    // 启动连接到C机器的线程
    if(pthread_create(&threadB, NULL, connect_to_server, &argsB) != 0) {
        perror("Failed to create thread for C");
        return -1;
    }

    // 等待两个线程完成
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    return 0;
}

