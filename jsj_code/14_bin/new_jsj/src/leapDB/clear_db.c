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

    redisReply *reply = redisCommand(ctx, "FLUSHALL");
    if (reply == NULL) {
        printf("Error: Failed to execute FLUSHALL command\n");
        redisFree(ctx);
        exit(EXIT_FAILURE);
    }

    printf("All data in leapDB have been flushed\n");

    freeReplyObject(reply);
    redisFree(ctx);

    return 0;
}

