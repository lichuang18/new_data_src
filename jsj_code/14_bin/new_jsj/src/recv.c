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
#define DEBUG 0

#define STORAGE "10.0.50.14"

#define TOTAL 100
#define NUM_VARS 5


//char time_str[30];
//char str[30];
//time_t now = time(NULL);struct tm *local = localtime(&now);
//strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
//printf("Current str time: %s\n", str);

struct message{
    int port; // 0 10Gb  1 FC  2 SRIO
    int signal;// 0 1 2 3 4  LD SAR IR RS SPEECH 
};

void generate_random_values(int values[], int num_values, int total) {
    int points[num_values - 1];
    int i;

    // 生成4个随机点
    for (i = 0; i < num_values - 1; ++i) {
        points[i] = rand() % (total + 1);
    }

    // 对这些点进行排序
    for (i = 0; i < num_values - 1; ++i) {
        for (int j = i + 1; j < num_values - 1; ++j) {
            if (points[i] > points[j]) {
                int temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }

    // 计算每个区间的长度
    values[0] = points[0];
    for (i = 1; i < num_values - 1; ++i) {
        values[i] = points[i] - points[i - 1];
    }
    values[num_values - 1] = total - points[num_values - 2];
}

int main(int argc, char *argv[]) {
    
    double compress_rate = 1;	
    if (argc < 4 || (strcmp(argv[1], "FC") != 0 && strcmp(argv[1], "10Gb") != 0 && strcmp(argv[1], "SRIO") != 0)) {
        printf("Usage: %s <Port> <Signal/multi> <Record Time>\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[2], "LD") != 0 && strcmp(argv[2], "SAR") != 0 && strcmp(argv[2], "IR") != 0 && strcmp(argv[2], "RS") != 0 && strcmp(argv[2], "SPEECH") != 0  && strcmp(argv[2], "multi") != 0 ) {
        printf("Signal only <LD/SAR/IR/RS/SPEECH/multi>\n");
        return 1;
    }
    if(argc == 5){
    	if(strcmp(argv[4], "compress") != 0){
		printf("Please Verify Compress Demand, and Input Right Param...\n");
		return 1;
    	}
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
    address.sin_port = htons(PORT);
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

    system("echo 'Try Connect Data Source' >> /home/record_log");
    if(DEBUG) printf("Try Connect Data Source\n");
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
    if(strcmp(argv[1], port_str[mes->port]) != 0){
	    printf("Data port link not match, Send port: %s\n",port_str[mes->port]);
            close(server_fd);
	    exit(1);
    }      

    if(strcmp(argv[2], signal_str[mes->signal]) != 0){
            printf("Data siganal not match, Send signal: %s\n",signal_str[mes->signal]);
	    close(server_fd);
            exit(1);
    }
   
    //The Flag of connect successful
    system("echo 'start record data' >> /home/record_log");
    //printf("Start Record Data\n");

    struct timeval start, end, end1;
    gettimeofday(&start, NULL);

    int time2 = atoi(argv[3]);

    if(DEBUG) printf("recv time: %d\n",time2);

    double execution_time = 0;

    //Create dir 
    char dir_command[100];
    sprintf(dir_command, "mkdir -p %s",argv[1]); // mkdir 10Gb
    system(dir_command);
    srand(time(NULL));


    //multi signal code  
    if(strcmp(argv[2], "multi") == 0){
	int values[NUM_VARS];
        // 生成随机值
        generate_random_values(values, NUM_VARS, TOTAL);
        const char *sign[] = {"LD", "SAR", "IR", "RS", "SPEECH"};
	double total_time =0;
	for(int i=0; i<5 ; i++)
        {
		char command[100];
    		while(execution_time - total_time <= (double)time2 * values[i] / 100){
        		double random_number = (double)(rand() % 1000) / 1000 * 0.3;
		        double u_time = (int)((2 + random_number) * 1000000);
		        double rand_speed = (double)(4 + random_number);
		        if(2 + random_number + execution_time - total_time >= (double)time2 * values[i] / 100){
                        	if(DEBUG) printf("tail time : %f , ",u_time);
			  	u_time = ((double)time2 * values[i] / 100 -  execution_time + total_time) * 1000000;
                        	if(DEBUG) printf("update tail time : %f \n",u_time);
			}
			usleep(u_time);
		        //time stamp
        		char str[30];
		        time_t now = time(NULL);struct tm *local = localtime(&now);
		        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
		        if(DEBUG) printf("Current str time: %s\n", str);
		        //Create file
		        if(DEBUG) printf("sp: %f , time: %f\n",rand_speed, u_time);
			if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
			if(DEBUG) printf("compress_rate : %f\n",compress_rate);
			sprintf(command, "fallocate -l %dM %s/%s_%s",(int)(rand_speed * u_time / 1000000 * 1024 * compress_rate),argv[1],sign[i],str);
		        system(command);
		        gettimeofday(&end, NULL);
		        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
		}
		total_time = execution_time;
	        if(DEBUG) printf("Single Signal Record Time: %f s\n",(double)time2 * values[i]);

        }
	char command_fp[100]; 
	sprintf(command_fp, "echo 'Multi Record Time: %f s' >> /home/record_log",execution_time);
	system(command_fp);

	close(server_fd);
   	return 0;
    }

    //single signal code
    char command[100];
    while(execution_time <= time2){
        double random_number = (double)(rand() % 1000) / 1000 * 0.3;
	double u_time = (int)((2 + random_number) * 1000000);
	double rand_speed = (double)(4 + random_number);
        usleep(u_time);
	//time stamp 
	char str[30];
	time_t now = time(NULL);struct tm *local = localtime(&now);
	strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
	if(DEBUG) printf("Current str time: %s\n", str);
        //Create file
	if(DEBUG) printf("sp: %f , time: %f\n",rand_speed, u_time);

	if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
        if(DEBUG) printf("compress_rate : %f\n",compress_rate);

	sprintf(command, "fallocate -l %dM %s/%s_%s 2>/dev/null",(int)((rand_speed * u_time / 1000000 * 1024) * compress_rate),argv[1],argv[2],str);
        system(command);
	gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    } 

    char command_sfp[100];
    sprintf(command_sfp, "echo 'Record Time: %f s' >> /home/record_log",execution_time);
    system(command_sfp);

    if(DEBUG) printf("Record Time: %f s\n",execution_time);

    if(DEBUG) printf("\n!!!!!!!!!!!!!!!!Welcome, mk!!!!!!!!!!!!!!!!!!\n");

    //char time_str[30];
    //char str[30];
    //time_t now = time(NULL);struct tm *local = localtime(&now);
    //strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
    //printf("Current str time: %s\n", str);

    close(server_fd);
    return 0;
}
