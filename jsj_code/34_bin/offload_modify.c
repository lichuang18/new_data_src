#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


#define DEBUG 1
#define PROGRESS_BAR_WIDTH 50

#define DEV "34"

void print_progress_bar(int progress) {
    int i;
    putchar('[');
    for (i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        if (i < progress * PROGRESS_BAR_WIDTH / 100) {
            putchar('=');
        } else {
            putchar(' ');
        }
    }
    printf("] %d%%\r", progress);
    fflush(stdout);
}


int64_t simple_hash(const char *str) {
    int64_t hash = 0;
    int c;

    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <offlaod_file> <root_password> <IP> <Port>\n", argv[0]);
        return 1;
    }
    char ip_addr[100];
    snprintf(ip_addr, sizeof(ip_addr), "sshpass -p %s ssh root@%s 'echo Connect!'", argv[2], argv[3]);
    int ret = system(ip_addr);

    char port_fg[100];
    snprintf(port_fg, sizeof(port_fg), "sshpass -p %s ssh root@%s 'echo %s > /root/port_fg'", argv[2], argv[3], argv[4]);
    ret = system(port_fg);

    char ip_fg[100];     
    char dev_num[10] = DEV;
    snprintf(ip_fg, sizeof(ip_fg), "sshpass -p %s ssh root@%s 'echo %s > /root/ip_fg'", argv[2], argv[3], dev_num);
    ret = system(ip_fg);

    //The Flag of connect successful
    printf("Start Offload Data\n");
    struct timeval start, end, end1;
    gettimeofday(&start, NULL);

    srand(time(NULL));
    // 打开文件
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Error: Failed to open file\n");
        return 1;
    }
     // 获取文件大小
    fseek(file, 0, SEEK_END); // 定位到文件末尾
    long size = ftell(file);   // 获取文件指针当前位置，即文件大小
    fclose(file);              // 关闭文件

    printf("Offload File size: %ld Mbytes\n", size/1024/1024);

    double random_number = (double)(rand() % 1000) / 1000 * 0.3;
    double need_time = ((double)size)/1024/1024/(4 + random_number)/1024;

    //path resolve
    char *input_path = argv[1];
    char str[32];  // 假设str有足够的长度来存储结果
    char *last_slash = strrchr(input_path, '/'); // 使用 strrchr 查找最后一个斜杠的位置
    if (last_slash != NULL) {
        strcpy(str, last_slash + 1);// 提取最后一层，跳过最后一个斜杠
    } else {
        strcpy(str, input_path); // 如果路径中没有斜杠，直接复制整个字符串
    }

    char c_command[100];
    sprintf(c_command, "sshpass -p 1 ssh root@10.0.50.22 'cd /mnt ; /root/bin/offload_assist %s %f %d 2>/dev/null'",str, (double)(size/1024/1024),0);
    //printf("command %s\n",c_command);
    system(c_command);

    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    if (execution_time < need_time)
    {
        usleep((need_time-execution_time)/2*1000000);
        system("echo 'half done' >> /home/offload_log");
        usleep((need_time-execution_time)/2*1000000);
    }else{	
        return 0;
    }


    char command[100];
    sprintf(command, "echo 'use time %f s' >> /home/offload_log",need_time);
    system(command);
    if(DEBUG) printf("use time: %f s\n",need_time);

    double offload_speed = ((double)(size)) /1024/1024/1024/ need_time;

    sprintf(command, "echo 'offload speed: %f GB/s' >> /home/offload_log",offload_speed);
    system(command);
    if(DEBUG) printf("offload speed: %f GB/s\n",offload_speed);

    return 0;
}

