#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>


#define DEBUG 0

#define REDIS_SERVER "127.0.0.1"
#define REDIS_PORT 6379
#define NUM_FRAME 2000000

// insert op

int main(int argc, char *argv[]) {
    
    //insert number signal

    redisContext *ctx = redisConnect(REDIS_SERVER, REDIS_PORT);
    if (ctx == NULL || ctx->err) {
        if (ctx) {
            printf("Error: %s\n", ctx->errstr);
            redisFree(ctx);
        } else {
            printf("Error: Failed to allocate leapDB context\n");
        }
        exit(EXIT_FAILURE);
    }

    printf("Connected to leapDB server\n");

    redisReply *reply;

    char key[64];

    int64_t sig_size = 0;

    if(strncmp(argv[2], "LD", 2) == 0){
        sig_size = 655360;//655360
    }else if(strncmp(argv[2], "SAR", 2) == 0){
        sig_size = 131072;//131072
    }else if(strncmp(argv[2], "IR", 2) == 0){
        sig_size = 262144;//262144
    }else if(strncmp(argv[2], "RS", 2) == 0){
        sig_size = 786432;//786432
    }else if(strncmp(argv[2], "SPEECH", 4) == 0){
        sig_size = 131072;//131072
    }else{
        printf("Error: wrong signal type \n");
        exit(-1);
    }    


    char value[64];
    sprintf(value, "%s_%ldMb",argv[2], sig_size/131072);// SAR_1M

    int64_t number = atoi(argv[1]);
    double parse_time = (double)number * sig_size / 131072 / 8 / 1024 / 4.4;

    struct timeval start, end, end1, start1;
    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    

    //for (int i = 0; i < number; ++i) {
    int i = 0;
    while(i < number && execution_time < parse_time){
	sprintf(key, "%lld", i * sig_size); //address
        char str[30];
	time_t now = time(NULL);struct tm *local = localtime(&now);
        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
	reply = redisCommand(ctx, "SET %d %s_%s_%s", i, value,key,str);
	//usleep(split_time);
        /*if (reply == NULL) {
            printf("Error: Failed to execute SET command\n");
            redisFree(ctx);
            exit(EXIT_FAILURE);
        }*/
	gettimeofday(&end, NULL);
	execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
	i++;  
        freeReplyObject(reply);
    }
    /*
    for (int i = 0; i < number; ++i) {
        sprintf(key, "%lld", i * sig_size); //address
        char str[30];
        time_t now = time(NULL);struct tm *local = localtime(&now);
        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
        reply = redisCommand(ctx, "SET %d %s_%s_%s", i, value,key,str);
        //usleep(split_time);
        freeReplyObject(reply);
    }*/
    redisFree(ctx);
    //printf("Inserted over!\n");
    return 0;
}

