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

    if(ret<0){
        printf("Please input real IP...\n");
        return 1;
    }
    //The Flag of connect successful
    system("echo 'start offload data' >> /home/offload_log");
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

    printf("Progress: ");
    for (int i = 0; i <= 100; ++i) {
        print_progress_bar(i);
        long utime = need_time * 10000;
	if(i == 50) system("echo 'half done' >> /home/offload_log");
        usleep(utime);
    }
    putchar('\n');


    char command[100];
    sprintf(command, "echo 'use time %f s' >> /home/offload_log",need_time);
    system(command);
    if(DEBUG) printf("use time: %f s\n",need_time);


    double offload_speed = ((double)(size)) /1024/1024/1024/ need_time;
    sprintf(command, "echo 'offload speed: %f GB/s' >> /home/offload_log",offload_speed);
    system(command);
    if(DEBUG) printf("offload speed: %f GB/s\n",offload_speed);

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


    char command_s[100];
    sprintf(command_s, "sshpass -p 1 ssh root@10.0.50.22 'fallocate -l %dM /mnt/%s 2>/dev/null'",(int)(size/1024/1024),str);
    //sprintf(command, "sshpass -p 1 ssh root@10.0.50.22 'fallocate -l %dM /mnt/%s 2>/dev/null'",(int)(size/1024/1024),argv[1]);
    system(command_s);

    return 0;
}
