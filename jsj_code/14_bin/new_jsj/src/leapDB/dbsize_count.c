#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>

#define REDIS_SERVER "127.0.0.1"
#define REDIS_PORT 6379

int main() {
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

    // 发送 DBSIZE 命令获取键的数量
    redisReply *reply = redisCommand(ctx, "DBSIZE");
    if (reply == NULL || reply->type != REDIS_REPLY_INTEGER) {
        printf("Error: Failed to execute DBSIZE command or invalid reply\n");
        redisFree(ctx);
        exit(EXIT_FAILURE);
    }

    printf("Total number of keys in the database: %lld\n", reply->integer);

    freeReplyObject(reply);
    redisFree(ctx);

    return 0;
}

