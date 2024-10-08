#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


#define PARSE_PORT 5253
#define PORT 5252
#define STORAGE_IP1 "10.0.50.14"
#define STORAGE_IP2 "10.0.50.34"
#define DEBUG 1

// 封装线程参数的结构体
typedef struct {
    const char *ip;
    char *param1;
    char *param2;
    struct message *mes;
    double run_time;
} ThreadArgs;

struct message{
    int port;  // 0 1 2  10Gb FC SRIO
    int signal; // 0 1 2 3 4 5  LD SAR IR RS SPEECH multi
};

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

    struct timeval start, end, end1, start1;
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
    gettimeofday(&start1, NULL);
    // 发送字符串
    if(threadArgs->ip == STORAGE_IP1){
       printf("\nConnect A_DEV Trans Data\n");
    }else if(threadArgs->ip == STORAGE_IP2){
       printf("\nConnect B_DEV Trans Data\n");
    }

    //const char *sth = "10Gb"; // 要传递的字符串
    char buffer[1024] = {0};
    int sockB, sockC;

    struct message *sth = (struct message *)buffer;
    
    sth->port = threadArgs->mes->port;
    sth->signal = threadArgs->mes->signal;
   
    send(sock, sth, sizeof(sth), 0);
   
    printf("\nString sent to %s_dev\n", threadArgs->param2);

    // 等待确认消息
    //read(sock, buffer, 1024);
    //if (strcmp(buffer, "received") == 0) {
    //    printf("\n%s_dev 成功\n", threadArgs->param2);
    //}

    gettimeofday(&end1, NULL);
    execution_time = (end1.tv_sec - start1.tv_sec) + (end1.tv_usec - start1.tv_usec) / 1000000.0;
    
    //printf("hello mk , executime %f\n",execution_time);
    
    double time2 = threadArgs->run_time;
    if(execution_time <= time2)
    {
            int sum=0;
            sum = (int)(((double)time2 - execution_time)*1000000);
            usleep(sum);
    }

    gettimeofday(&end1, NULL);
    execution_time = (end1.tv_sec - start1.tv_sec) + (end1.tv_usec - start1.tv_usec) / 1000000.0;
    printf("Trans Time %s_dev , %fs\n",threadArgs->param2,execution_time);


    close(sock);

}

void *connect_to_parse(void *args) {

    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return NULL;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PARSE_PORT);

    if(inet_pton(AF_INET, threadArgs->ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return NULL;
    }

    struct timeval start, end, end1, start1;
    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    while(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 && execution_time < 10){
        gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("No Target data_parse recv Data %s , %fs\r",threadArgs->param2,execution_time);
    }
    //printf("\n");
    if(execution_time >= 10){
         printf("\n%s_dev data_parse recv timeout\n",threadArgs->param2);
         return NULL;
    }

    gettimeofday(&start1, NULL);
    // 发送字符串
    if(threadArgs->ip == STORAGE_IP1){
       printf("\nConnect A_DEV Data Parse\n");
    }else if(threadArgs->ip == STORAGE_IP2){
       printf("\nConnect B_DEV Data Parse\n");
    }
    //const char *sth = "10Gb"; // 要传递的字符串
    char buffer[1024] = {0};
    int sockB, sockC;

    struct message *sth = (struct message *)buffer;

    sth->port = threadArgs->mes->port;
    sth->signal = threadArgs->mes->signal;

    send(sock, sth, sizeof(sth), 0);

    printf("\nString sent to %s_dev for data_parse\n", threadArgs->param2);
    double time2 = threadArgs->run_time;

    gettimeofday(&end1, NULL);
    execution_time = (end1.tv_sec - start1.tv_sec) + (end1.tv_usec - start1.tv_usec) / 1000000.0;

    //printf("hello mk , executime %f\n",execution_time);


    if(execution_time <= time2)
    {
            int sum=0;
            sum = (int)(((double)time2 - execution_time)*1000000);
            usleep(sum);
    }

    gettimeofday(&end1, NULL);
    execution_time = (end1.tv_sec - start.tv_sec) + (end1.tv_usec - start.tv_usec) / 1000000.0;
    printf("Data_parse Time %s_dev , %fs\n",threadArgs->param2,execution_time);


    close(sock);

}


int main(int argc, char *argv[]) {

    if (argc < 4 || (strcmp(argv[1], "FC") != 0 && strcmp(argv[1], "10Gb") != 0 && strcmp(argv[1], "SRIO") != 0)) {
        printf("Usage: %s <Port> <Signal> <Record Time>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "LD") != 0 && strcmp(argv[2], "SAR") != 0 && strcmp(argv[2], "IR") != 0 && strcmp(argv[2], "RS") != 0 && strcmp(argv[2], "SPEECH") != 0 && strcmp(argv[2], "multi") != 0) {
        printf("Signal only <LD/SAR/IR/RS/SPEECH/multi>\n");
        return 1;
    }

    if(argc == 5){
        if(strcmp(argv[4], "data_parse") != 0){
                printf("Please Verify data_parse Demand, and Input Right Param...\n");
                return 1;
        }
    }



    char buffer[1024] = {0};
    struct message *mes = (struct message *)buffer;

    if(strncmp(argv[1], "10Gb", 2) == 0){
        mes->port = 0;
    }else if(strncmp(argv[1], "FC", 2) == 0){
        mes->port = 1;
    }else if(strncmp(argv[1], "SRIO", 2) == 0){
        mes->port = 2;
    }else{
        printf("Error: wrong port type \n");
        exit(-1);
    }

    if(strncmp(argv[2], "LD", 2) == 0){
        mes->signal = 0;
    }else if(strncmp(argv[2], "SAR", 2) == 0){
        mes->signal = 1;
    }else if(strncmp(argv[2], "IR", 2) == 0){
        mes->signal = 2;
    }else if(strncmp(argv[2], "RS", 2) == 0){
        mes->signal = 3;
    }else if(strncmp(argv[2], "SPEECH", 4) == 0){
        mes->signal = 4;
    }else if(strncmp(argv[2], "multi", 4) == 0){
        mes->signal = 5;
    }else{
        printf("Error: wrong signal type \n");
        exit(-1);
    }


    pthread_t threadB, threadA,threadPA, threadPB;
    //struct ThreadArgs argsB, argsC;
    //

    ThreadArgs argsA = {"ip2", "10Gb", "A", mes, 30}; // 参数1和参数2是示例数据
    ThreadArgs argsB = {"ip3", "10Gb", "B", mes, 30};
    
    argsA.ip = STORAGE_IP1;
    argsA.param1 = argv[1];
    argsA.param2 = "A";
    argsA.run_time = (double)atoi(argv[3]);


    argsB.ip = STORAGE_IP2;
    argsB.param1 = argv[1];
    argsB.param2 = "B";
    argsB.run_time = (double)atoi(argv[3]);



    if(argc == 5){
        printf("@@@@@ hello mk! @@@@\n");
	// start thread for A_dev data parse
        if(pthread_create(&threadPA, NULL, connect_to_parse, &argsA) != 0) {
            perror("Failed to create thread for A_DEV Data Parse");
            return -1;
        }
	// start thread for B_dev data parse
        if(pthread_create(&threadPB, NULL, connect_to_parse, &argsB) != 0) {
            perror("Failed to create thread for B_DEV Data Parse");
            return -1;
        }
    }

    // start thread for A_dev data trans
    if(pthread_create(&threadA, NULL, connect_to_server, &argsA) != 0) {
        perror("Failed to create thread for A_DEV Data Trans");
        return -1;
    }

    // start thread for B_dev data trans
    if(pthread_create(&threadB, NULL, connect_to_server, &argsB) != 0) {
        perror("Failed to create thread for B_DEV Data Trans");
        return -1;
    }

    // 等待两个线程完成
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    if(argc == 5){
        pthread_join(threadPA, NULL);
        pthread_join(threadPB, NULL); 
    }

    return 0;
}

