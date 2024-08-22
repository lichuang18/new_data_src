#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define PORT 5252
#define PARSE_PORT 5253
#define DEBUG 1

#define STORAGE "10.0.50.4"

struct message{
    int port; // 0 10Gb  1 FC  2 SRIO
    int signal;// 0 1 2 3 4  LD SAR IR RS SPEECH
};

int main(int argc, char *argv[]) {
    // 调用Shell命令
    struct timeval start, end, end1;

    srand(time(NULL));
    if(argc < 2)
    {
            printf("please input analyse time...\n");
            printf("%s <option> analyse_time/version/clearDB/dbsize/search\n",argv[0]);
            return 1;
    }
    if(argc == 3 && strcmp(argv[1], "search") == 0)
    {
	    int addr = atoi(argv[2]);
	    char command[100];
	    sprintf(command, "/root/bin/leapDB_bin/leapDB_select %d",addr);
	    int ret = system(command);
            return 0;
    }

    if(strcmp(argv[1], "dbsize") == 0 ){
    	    //int ret = system("./leapDB_bin/leapDB_clear_db");
    	    int ret = system("/root/bin/leapDB_bin/leapDB_dbsize");
	    
            return 0;
    }

    if(strcmp(argv[1], "clearDB") == 0 ){
            //int ret = system("./leapDB_bin/leapDB_clear_db");
            int ret = system("/root/bin/leapDB_bin/leapDB_clear_db");

            return 0;
    }
    
    if(strcmp(argv[1], "search") == 0 ){
            //int ret = system("./leapDB_bin/leapDB_clear_db");
            int ret = system("/root/bin/leapDB_bin/leapDB_show");
            ret = system("/root/bin/leapDB_bin/leapDB_dbsize");
            return 0;
    }


    if(strcmp(argv[1], "version") == 0 ){
	    printf("TsinghuaStorage leapDB v1.0.1\n");
	    return 0;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // Creat socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PARSE_PORT);
    // Bind Socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Try Connect Data Source\n");
     // wait connect
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // read flag from data src   //bug
    read(new_socket, buffer, 1024);

    struct message *mes = (struct message *)buffer;
    if(DEBUG) printf("data src port is : %d\n",mes->port);
    if(DEBUG) printf("data src signal is : %d\n",mes->signal);

    char *port_str[] = {
            "10Gb",
            "FC",
            "SRIO"
    };
    char *signal_str[] = {
            "LD",
            "SAR",
            "IR",
            "RS",
            "SPEECH",
            "multi"
    };
    //To verify port and signal match condition
    if(DEBUG) printf("Data port link not match, Send port: %s\n",port_str[mes->port]);
    if(DEBUG) printf("Data siganal not match, Send signal: %s\n",signal_str[mes->signal]);

    if(signal_str[mes->signal] == "multi"){
   	 printf("leapDB DO not SUPPORT multi...\n");
	 return -1;
    }

    //The Flag of connect successful
    printf("Start Data Parse\n");

    gettimeofday(&start, NULL);

    double random_number = (double)(rand() % 1001) / 1000 * 0.3;
    //printf("rand: %.3f\n",random_number);
    int time2 = atoi(argv[1]);
    double total_send_data = (double)(4 + random_number) * time2;  //trans data capa


    double sig_size = 0;

    if(strncmp(signal_str[mes->signal], "LD", 2) == 0){
        sig_size = 5;
    }else if(strncmp(signal_str[mes->signal], "SAR", 2) == 0){
        sig_size = 1;
    }else if(strncmp(signal_str[mes->signal], "IR", 2) == 0){
        sig_size = 2;
    }else if(strncmp(signal_str[mes->signal], "RS", 2) == 0){
        sig_size = 6;
    }else if(strncmp(signal_str[mes->signal], "SPEECH", 4) == 0){
        sig_size = 1;
    }else{
        printf("Error: wrong signal type \n");
        exit(-1);
    }

    int64_t insert_number = (int64_t)(total_send_data * 1024 * 8 / sig_size);
    char command[100];

    sprintf(command, "/root/bin/leapDB_bin/leapDB_insert_N %ld %s", insert_number, signal_str[mes->signal]);
    int ret = system(command);
    
    gettimeofday(&end, NULL);
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    
    if (execution_time < time2)
    {
	    sleep((time2-(int)execution_time));
    }else{	
    	//printf("Analyse time : %ds, 分析帧数量: %ld\n", execution_time, insert_number);
    	int re =  system("/root/bin/leapDB_bin/perf_insert |grep \"per second\"");
    	return 0;
    }
    //printf("analyse time : %ds, 分析帧数量: %ld\n", time2, insert_number);
    // 检查返回值
    int re =  system("/root/bin/leapDB_bin/perf_insert |grep \"per second\" | sed -E 's/^([0-9]+)\\.[0-9]+/\\1/' ");
    return 0;
}

