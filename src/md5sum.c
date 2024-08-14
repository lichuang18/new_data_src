#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // 包含 PATH_MAX
#include <unistd.h>  // 包含 realpath 函数

#define MD5_DIGEST_LENGTH 16


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char resolved_path[PATH_MAX];

    // 使用 realpath 解析绝对路径
    if (realpath(filename, resolved_path) == NULL) {
        perror("realpath");
        return 1;
    }

    // 检查文件是否位于 /mnt 目录下
    if (strncmp(resolved_path, "/mnt", 4) == 0) {
        printf("hello\n");
        //ssh root@10.0.50.22  'md5sum /mnt/LD '
	return 0;
    }else{
    	char command[1024] = "md5sum";
        for (int i = 1; i < argc; i++) {
            strcat(command, " ");
            strcat(command, argv[i]);
        }
        // 调用系统命令 md5sum
        int result = system(command);
        if (result == -1) {
            perror("system");
            return 1;
        }
    }

    return 0;
}

