#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>

#define PORT 5252
#define PARSE_PORT 5253
#define DEBUG 0
#define DEBUG_FILE_SIZE 0

#define STORAGE "10.0.50.34"

#define TOTAL 100
#define NUM_VARS 5

int64_t simple_hash(const char *str) {
    int64_t hash = 0;
    int c;

    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash;
}

typedef struct {
    //dev stat
    uint16_t field1;
    uint16_t field2;
    uint8_t field3;
    uint8_t field4;
    uint16_t field5;
    uint16_t field6;
    uint8_t field7;
    uint8_t field8;
    uint8_t field9;
    uint8_t field10;
    uint8_t field11;
    uint8_t field12;
    uint8_t field13[14];
    uint8_t field14;
    uint8_t field15;
    uint8_t field16;
    uint8_t field17[11];
    uint16_t field18;
    uint16_t field19;
    uint16_t field20;
    //record ...
    unsigned long field21;
    unsigned long field22;
    unsigned char field23;
    unsigned char field24;
    unsigned short field25;
    unsigned char field26;
    unsigned char field27;
    unsigned short field28;
    uint8_t field29[14];
    unsigned long field30;
    char rand_data[100];
} LDSignal;

void assign_random_values_ld(LDSignal *signal, int64_t sed) {
    srand(sed);
    signal->field1 = 1;
    signal->field2 = 2;
    signal->field3 = 3;
    signal->field4 = 4;
    signal->field5 = 5;
    signal->field6 = 6;
    signal->field7 = 7;
    signal->field8 = 8;
    signal->field9 = 9;
    signal->field10 = 10;
    signal->field11 = 11;
    signal->field12 = 12;
    memset(signal->field13, 0, sizeof(signal->field13));  // Initialize array to zeros
    signal->field14 = 14;
    signal->field15 = 15;
    signal->field16 = 16;
    memset(signal->field17, 0, sizeof(signal->field17));  // Initialize array to zeros
    signal->field18 = 18;
    signal->field19 = 19;
    signal->field20 = 20;
    signal->field21 = 21;// Filling record fields with example data
    signal->field22 = 22;
    signal->field23 = 23;
    signal->field24 = 24;
    signal->field25 = 25;
    signal->field26 = 26;
    signal->field27 = 27;
    signal->field28 = 28;
    memset(signal->field29, 0, sizeof(signal->field29));  // Initialize array to zeros
    signal->field30 = 30;

    for(int32_t i = 0; i <= 100; i++){
        signal->rand_data[i] = rand() % 1000;
    }
}

typedef struct {
    // TIFF Header
    char byte_order[2];       // Byte order indication: "II" or "MM"
    uint16_t fixed_number;    // Fixed number: 42
    uint32_t first_ifd_offset;// Offset to the first IFD
    uint16_t num_ifd_entries; // Number of entries in IFD
    char ifd_entries[10 * 12]; // IFD entries, 10 entries each of 12 bytes
    uint32_t next_ifd_offset; // Offset to the next IFD (0 if none)
    char rand_data[100];
} SARSignal;

void assign_random_values_sar(SARSignal *signal, int64_t sed) {
    srand(sed);
    signal->byte_order[0] = 'I'; // Little-endian
    signal->byte_order[1] = 'I';
    signal->fixed_number = 42;
    signal->first_ifd_offset = 8;
    signal->num_ifd_entries = 10;
    memset(signal->ifd_entries, 12, sizeof(signal->ifd_entries));
    signal->ifd_entries[0] = 0x01;  // Just an example tag
    signal->ifd_entries[1] = 0x02;  // Example type
    signal->ifd_entries[2] = 0x00;  // Example value/offset
    signal->next_ifd_offset = 0;
    for(int32_t i = 0; i <= 100; i++){
        signal->rand_data[i] = rand() % 1000;
    }
}


typedef struct {
    char byte_order[2];       // Byte order indication: "II" or "MM"
    uint16_t fixed_number;    // Fixed number: 42
    uint32_t first_ifd_offset;// Offset to the first IFD
    uint16_t num_ifd_entries; // Number of entries in IFD
    char ifd_entries[10 * 12]; // IFD entries, 10 entries each of 12 bytes
    uint32_t next_ifd_offset; // Offset to the next IFD (0 if none)
    char rand_data[100];
} IRSignal;

void assign_random_values_ir(IRSignal *signal, int64_t sed) {
    srand(sed);
    signal->byte_order[0] = 'I';
    signal->byte_order[1] = 'I';
    signal->fixed_number = 42;
    signal->first_ifd_offset = 8;
    signal->num_ifd_entries = 10;
    memset(signal->ifd_entries, 0, sizeof(signal->ifd_entries));
    signal->ifd_entries[0] = 0x01;  // Just an example, normally this would be a specific tag
    signal->ifd_entries[1] = 0x02;  // Another example byte
    signal->next_ifd_offset = 0;
    for(int32_t i = 0; i <= 100; i++){
        signal->rand_data[i] = rand() % 1000;
    }
}

typedef struct {
    // TIFF Header
    char byte_order[2];       // Byte order indication: "II" or "MM"
    uint16_t fixed_number;    // Fixed number: 42
    uint32_t first_ifd_offset;// Offset to the first IFD
    uint16_t num_ifd_entries; // Number of entries in IFD
    char ifd_entries[10 * 12]; // IFD entries, 10 entries each of 12 bytes
    uint32_t next_ifd_offset; // Offset to the next IFD (0 if none)
    char rand_data[100];
} RSSignal;

void assign_random_values_rs(RSSignal *signal, int64_t sed) {
    srand(sed);
    signal->byte_order[0] = 'I';
    signal->byte_order[1] = 'I';
    signal->fixed_number = 42;
    signal->first_ifd_offset = 8;
    signal->num_ifd_entries = 10;
    for (int i = 0; i < 10 * 12; i++) {
        signal->ifd_entries[i] = i;  // Fill with zeros or some example data
    }
    signal->next_ifd_offset = 0;
    for(int32_t i = 0; i <= 100; i++){
        signal->rand_data[i] = rand() % 1000;
    }
}

typedef struct {
    char riff_tag[4];              // "RIFF"
    int32_t file_length;           // 文件长度
    char wave_tag[4];              // "WAVE"
    char fmt_tag[4];               // "fmt "
    int32_t fmt_length;            // 格式块长度
    int16_t audio_format;          // 编码格式标识
    int16_t num_channels;          // 声道个数
    int32_t sample_rate;           // 采样频率
    int32_t byte_rate;             // 数据传输速率
    int16_t block_align;           // 数据块对齐单位
    int16_t bits_per_sample;       // 采样位数
    char data_tag[4];              // "data"
    int32_t data_length;           // 数据长度
    //char padding[131072 - 44];     // 填充到 131072 字节
    char rand_data[100];
} SPEECHSignal;

void assign_random_values_speech(SPEECHSignal *header, int64_t sed) {
    srand(sed);
    strcpy(header->riff_tag, "RIFF");
    header->file_length = 123456; // Just an example size
    strcpy(header->wave_tag, "WAVE");
    strcpy(header->fmt_tag, "fmt ");
    header->fmt_length = 16;
    header->audio_format = 1; // PCM format
    header->num_channels = 2;
    header->sample_rate = 44100;
    header->byte_rate = header->sample_rate * header->num_channels * 2;
    header->block_align = header->num_channels * 2;
    header->bits_per_sample = 16;
    strcpy(header->data_tag, "data");
    header->data_length = rand() % 131025;//654321; // Just an example size
    //
    for(int32_t i = 0; i <= 100; i++){
        header->rand_data[i] = rand() % 1000;
    }
}

void fill_file(double use_time, char *filename1, char *signal,char *filename0){ // speed =4.2  fixed
    char filename[100];
    sprintf(filename,"%s/%s",filename0,filename1);
    if(DEBUG) printf("file_name %s\n",filename);
    if(strcmp(signal, "LD") == 0){
        LDSignal header;
        int64_t sed = simple_hash(filename1);
	if(DEBUG) printf("filename %s sed %ld,use_time %f\n",filename, sed, use_time);
        if(use_time == (double)10 / 4.2){
	    if(DEBUG) printf("I am in ?\n");
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
	    if (fd < 0) {
                perror("Failed to open file");exit(EXIT_FAILURE);
            }
            for(int i = 0 ; i < 16*1024 ; i++){
                assign_random_values_ld(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(LDSignal), i*131072*5) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }else if(use_time < (double)10 / 4.2){  //执行end
            double file_size = use_time * 4.2;//how many GB
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            if (fd < 0) {
                perror("Failed to open file");exit(EXIT_FAILURE);
            }
            for(int i = 0 ; i < 8*1024*file_size/5 ; i++){
                assign_random_values_ld(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(LDSignal), i*131072*5) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }else if(strcmp(signal, "SAR") == 0){
        SARSignal header;
        int64_t sed = simple_hash(filename1);
        if(use_time == (double)10 / 4.2){
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 80*1024 ; i++){
                assign_random_values_sar(&header, sed + i);
                if (pwrite64(fd, &header, sizeof(SARSignal), i*131072) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }else if(use_time < (double)10 / 4.2){  //执行end
            double file_size = use_time * 4.2;//how many GB
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 8*1024*file_size ; i++){
                assign_random_values_sar(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(SARSignal), i*131072) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }else if(strcmp(signal, "IR") == 0){
        IRSignal header;
        int64_t sed = simple_hash(filename1);
        if(use_time == (double)10 / 4.2){
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 40*1024 ; i++){
                assign_random_values_ir(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(IRSignal), i*131072*2) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }else if(use_time < (double)10 / 4.2){  //执行end
            double file_size = use_time * 4.2;//how many GB
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 4*1024*file_size ; i++){
                assign_random_values_ir(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(IRSignal), i*131072*2) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }else if(strcmp(signal, "RS") == 0){
        RSSignal header;
        int64_t sed = simple_hash(filename1);
        if(use_time == (double)10 / 4.2){
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 80*1024/6 ; i++){
                assign_random_values_rs(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(RSSignal), i*131072*6) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }else if(use_time < (double)10 / 4.2){  //执行end
            double file_size = use_time * 4.2;//how many GB
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 8*1024*file_size ; i++){
                assign_random_values_rs(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(RSSignal), i*131072*6) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }else if(strcmp(signal, "SPEECH") == 0){
        SPEECHSignal header;
        int64_t sed = simple_hash(filename1);
        if(use_time == (double)10 / 4.2){
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 80*1024 ; i++){
                assign_random_values_speech(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(SPEECHSignal), i*131072) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }else if(use_time < (double)10 / 4.2){  //执行end
            double file_size = use_time * 4.2;//how many GB
            int fd = open(filename, O_RDWR | O_CREAT, 0666);
            for(int i = 0 ; i < 8*1024*file_size ; i++){
                assign_random_values_speech(&header, sed + i);
                //printf("[%d]: start write rand\n",i);
                if (pwrite64(fd, &header, sizeof(SPEECHSignal), i*131072) < 0) {
                    perror("Failed to pwrite");
                    close(fd);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

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
    printf("Start Record Data\n");
    system("echo 'start record data' >> /home/record_log");
    struct timeval start, end, end1;
    gettimeofday(&start, NULL);
    int time2 = atoi(argv[3]);
    if(DEBUG) printf("recv time: %d\n",time2);
    double execution_time = 0;
    //Create dir 
    char dir_command[100];
    sprintf(dir_command, "mkdir -p %s",argv[1]); // mkdir 10Gb
    system(dir_command);
    //multi signal code  
    if(strcmp(argv[2], "multi") == 0){
	    int values[NUM_VARS];
        // 生成随机值
        generate_random_values(values, NUM_VARS, TOTAL);
        char *sign[] = {"LD", "SAR", "IR", "RS", "SPEECH"};
	    double total_time =0;
        char command[100];
        char file_name[100];
	    for(int i=0; i<5 ; i++){
            double exec_time = (double)time2 * values[i] / 100 ;
	    if(DEBUG) printf("############hello mk!!  exec_time value[%d] %f #############\n",i, exec_time);
            //for full time record file
            while(exec_time >= (double)10/4.2){
                gettimeofday(&start, NULL);
                char str[30];
		        time_t now = time(NULL);struct tm *local = localtime(&now);
		        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
			    sprintf(file_name, "%s_%s",sign[i],str);
                int64_t sed = simple_hash(file_name);
                srand(sed);
                if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
			    if(DEBUG) printf("compress_rate : %f\n",compress_rate);
                double random_number = (double)(rand() % 1000) / 1000 * 0.02;
                sprintf(command, "fallocate -l %fM %s/%s;", 10240*compress_rate*(1 + random_number),argv[1],file_name);

                system(command);
                fill_file((double)10/4.2, file_name, sign[i],argv[1]);
		        gettimeofday(&end, NULL);
		        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

                if (execution_time < (double)10/4.2)
                {
                    usleep(((double)10/4.2-execution_time)*1000000);
                }else{	
                    printf("&&&&&&&&&&&&&&&&  record file timeout %f &&&&&&&&&&&&&&&&&&&\n",execution_time);
                    return 0;
                }
                exec_time = exec_time - (double)10/4.2;
		if(DEBUG) printf("exec_st %s, sleep time  %f, may be timeout %f\n",str,(double)10/4.2-execution_time,execution_time);
            }
            //for tail
            if(exec_time < (double)10/4.2){
                gettimeofday(&start, NULL);
                double tail_filesize = exec_time * 4.2; //GB
                char str[30];
		        time_t now = time(NULL);struct tm *local = localtime(&now);
		        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
			    sprintf(file_name, "%s_%s",sign[i],str);
                int64_t sed = simple_hash(file_name);
                srand(sed);
                if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
			    if(DEBUG) printf("compress_rate : %f\n",compress_rate);
                double random_number = (double)(rand() % 1000) / 1000 * 0.02;
                sprintf(command, "fallocate -l %fM %s/%s;", tail_filesize*compress_rate*(1+random_number),argv[1],file_name);
                system(command);
                fill_file(exec_time, file_name, sign[i],argv[1]);
		        gettimeofday(&end, NULL);
		        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
                if (execution_time < exec_time)
                {
                    usleep((exec_time-execution_time)*1000000);
                }else{	
                    printf("&&&&&&&&&&&&&&&&  record file timeout %f &&&&&&&&&&&&&&&&&&&\n",execution_time);
                    return 0;
                }
		if(DEBUG) printf("exec _ %s, sleep time  %f, timeout %f\n",str,exec_time-execution_time,execution_time);
                if(DEBUG) printf("test over\n");
            }

        }
        printf("Multi Record Time: %f s\n",execution_time);
        close(server_fd);
        return 0;
    }


    //single signal code
    char command[100];
    char file_name[50];
    double exec_time = time2;

    if(DEBUG) printf("single signal exec_time %f\n",exec_time);
    //for full time record file
    while(exec_time >= (double)10/4.2){
        gettimeofday(&start, NULL);
        char str[30];
        time_t now = time(NULL);struct tm *local = localtime(&now);
        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
        sprintf(file_name, "%s_%s",argv[2],str);
        int64_t sed = simple_hash(file_name);
        srand(sed);
        if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
        if(DEBUG) printf("compress_rate : %f\n",compress_rate);
        double random_number = (double)(rand() % 1000) / 1000 * 0.02;
        sprintf(command, "fallocate -l %fM %s/%s;", 10240*compress_rate*(1+random_number),argv[1],file_name);
	if(DEBUG_FILE_SIZE) printf("file %s , size: %f\n",file_name, 10240*compress_rate*(1+random_number));
        system(command);
        fill_file((double)10/4.2, file_name, argv[2],argv[1]);
        gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

        if (execution_time < (double)10/4.2)
        {
            usleep(((double)10/4.2-execution_time)*1000000);
        }else{	
            printf("&&&&&&&&&&&&&&&&  record file timeout %f &&&&&&&&&&&&&&&&&&&\n",execution_time);
            return 0;
        }
        exec_time = exec_time - (double)10/4.2;
	if(DEBUG) printf("exec_st %s, sleep time  %f, may be timeout %f\n",str,(double)10/4.2-execution_time,execution_time);
    }
    //for tail
    if(exec_time < (double)10/4.2){
        gettimeofday(&start, NULL);
        double tail_filesize = exec_time * 4.2; //GB
        char str[30];
        time_t now = time(NULL);struct tm *local = localtime(&now);
        strftime(str, sizeof(str), "%Y%m%d_%H%M%S", local);
        sprintf(file_name, "%s_%s",argv[2],str);
        int64_t sed = simple_hash(file_name);
        srand(sed);
        if(argc == 5) compress_rate = (double)(1 - (double)(rand() % 1000) / 1000 * 0.2); //8x%
        if(DEBUG) printf("compress_rate : %f\n",compress_rate);
        double random_number = (double)(rand() % 1000) / 1000 * 0.02;
        sprintf(command, "fallocate -l %fM %s/%s;", tail_filesize*compress_rate*(1+random_number),argv[1],file_name);
        system(command);
	if(DEBUG_FILE_SIZE) printf("file %s , size: %f\n",file_name, tail_filesize*compress_rate*(1+random_number));
        fill_file(exec_time, file_name, argv[2],argv[1]);
        gettimeofday(&end, NULL);
        execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        if (execution_time < exec_time)
        {
            usleep((exec_time-execution_time)*1000000);
        }else{	
            printf("&&&&&&&&&&&&&&&&  record file timeout %f &&&&&&&&&&&&&&&&&&&\n",execution_time);
            return 0;
        }
	if(DEBUG) printf("exec _ %s, sleep time  %f, timeout %f\n",str,exec_time-execution_time,execution_time);
        if(DEBUG) printf("test over\n");
    }

    close(server_fd);
    return 0;
}

