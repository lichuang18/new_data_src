#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // 包含 PATH_MAX
#include <unistd.h>  // 包含 realpath 函数

#define MD5_DIGEST_LENGTH 16
#define STORAGE_IP "10.0.50.4"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "用法: %s <文件名>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char resolved_path[PATH_MAX];


    FILE *file;
    char strrr[20];  // 假设文件内容不超过255个字符
    char *fname = "/root/port_fg";

    // 打开文件，"r"表示以只读方式打开
    file = fopen(fname, "r");
    if (file == NULL) {
        //perror("Error opening file");
        return 1;
    }

    // 读取文件内容到字符串变量strrr
    if (fgets(strrr, sizeof(strrr), file) != NULL) {
        // fgets会读取一行内容并存储到strrr中
        //printf("File content: %s\n", strrr);
    }
    fclose(file);



    // 使用 realpath 解析绝对路径
    if (realpath(filename, resolved_path) == NULL) {
        perror("realpath");
        return 1;
    }

    //path resolve
    char *input_path = argv[1];
    char str[32];  // 假设str有足够的长度来存储结果

    // 使用 strrchr 查找最后一个斜杠的位置
    char *last_slash = strrchr(input_path, '/');

    if (last_slash != NULL) {
        // 提取最后一层，跳过最后一个斜杠
        strcpy(str, last_slash + 1);
    } else {
        // 如果路径中没有斜杠，直接复制整个字符串
        strcpy(str, input_path);
    }


    char storage_ip[40] = STORAGE_IP;
    // 检查文件是否位于 /mnt 目录下
    if (strncmp(resolved_path, "/mnt", 4) == 0) {
	char command[200];
        sprintf(command, "sshpass -p 1 ssh %s 'md5sum /mnt/pool1/fs1/%s/%s 2>/dev/null'",storage_ip, strrr, str);
        //sprintf(command, "sshpass -p 1 ssh %s 'md5sum /root/new_jsj/10Gb/%s 2>/dev/null'",storage_ip, str);
	system(command);
	//printf("comand : %s  path-str : %s\n",command, str);
	return 0;
    }else{
    	char command[1024] = "old_md5sum";
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

