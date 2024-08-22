#include <stdio.h>

int main() {
    FILE *fp;
    char path[1035];

    // 执行redis-benchmark命令，只输出SET操作的统计数据
    fp = popen("redis-benchmark -t set", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        return 1;
    }

    // 读取命令输出并打印
    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("%s", path);
    }

    // 关闭文件指针
    pclose(fp);

    return 0;
}

