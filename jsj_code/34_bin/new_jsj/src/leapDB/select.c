#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <string.h>
#define DEBUG 1

#define REDIS_SERVER "127.0.0.1"
#define REDIS_PORT 6379

// insert op
//
int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <key>\n", argv[0]);
	//printf("argument:\n\t1-RADAR\n\t2-SAR\n\t3-RED\n\t4-REMOTE\n\t5-VOICE\n");
        return 1;
    }

	
    if(DEBUG) printf("Argument: %s\n", argv[1]);

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

    //const char *key = "name";

    // 发送 GET 命令获取指定的 key 的值
    redisReply *reply = redisCommand(ctx, "GET %s", argv[1]);
    if (reply == NULL) {
        printf("Error: Failed to execute GET command\n");
        redisFree(ctx);
        exit(EXIT_FAILURE);
    }

    if (reply->type == REDIS_REPLY_STRING) {
        printf("Value of key \"%s\": %s\n", argv[1], reply->str);
    } else {
        printf("Key \"%s\" not found\n", argv[1]);
    }

    freeReplyObject(reply);
    redisFree(ctx);

    return 0;
}

