#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "extra.h"

int funct(char *name){
	FILE *f=fopen(name,"r");	//theloume mono diavasma tou arxeioy kai oxi allagi tou
	char c;
	int sum=0;
	while((c=fgetc(f))!=EOF){
		if(c=='\n'){	//vrisketai sto telos kathe grammis, 
			sum++;
		}
	}
	fclose(f);
	printf("file has:%d lines\n",sum);
	return sum;
}

void funct2(char *name,int num,int *pos){
	FILE *f=fopen(name,"r");
	int siz=101,count=1;
	char* line = malloc(siz);
	while (fgets(line,siz,f)!=NULL){
		if(count==num){
			printf("parent to child:%s\n",line);
			strcpy((char*)(pos+sizeof(int)+3*sizeof(sem_t)),line);
		}
		count++;
	}
	fclose(f);
}	
