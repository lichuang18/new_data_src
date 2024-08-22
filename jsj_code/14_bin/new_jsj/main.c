#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

        srand(12345);
        int buffer[50];
        for(int i=0; i<50;i++){
        buffer[i] = rand() % 256;
        printf("%d ",buffer[i]);
        }
        printf("\n");

	for(int i = 0 ; i<=4 ; i++){
                srand(100+i);
                int64_t value = rand()%25;
                printf("%ld ",value);
        }

        printf("\n");
        return 0;
}
