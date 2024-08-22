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
	
    // 设置游标初始值为0
    int cursor = 0;

    // 迭代所有的键
    do {
        // 执行 SCAN 命令获取一批键
        redisReply *reply = redisCommand(ctx, "SCAN %d", cursor);
        if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
            printf("Error: Failed to execute SCAN command or invalid reply\n");
            redisFree(ctx);
            exit(EXIT_FAILURE);
        }

        // 获取下一个游标值
        cursor = atoi(reply->element[0]->str);

        // 遍历当前批次的键
        for (size_t i = 0; i < reply->element[1]->elements; ++i) {
            const char *key = reply->element[1]->element[i]->str;

            // 获取键的值
            redisReply *value_reply = redisCommand(ctx, "GET %s", key);
            if (value_reply == NULL || value_reply->type != REDIS_REPLY_STRING) {
                printf("Error: Failed to execute GET command for key \"%s\"\n", key);
            } else {
                printf("%s: %s\n", key, value_reply->str);
            }
            freeReplyObject(value_reply);
        }

        freeReplyObject(reply);
    } while (cursor != 0); // 如果游标值为0，则表示迭代结束


    redisFree(ctx);

    return 0;
}

