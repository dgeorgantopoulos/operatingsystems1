#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "extra.h"

int main(int argc,char **argv){
	clock_t cl;
	int shmid,status;
	int SHMSIZE=sizeof(int)+3*sizeof(sem_t)+101*sizeof(char);

	if(argc!=4){
		printf("you have entered the wrong number of arguments");
		return 0;
	}
	int k=atoi(argv[2]);
	int n=atoi(argv[3]);
	int lines=funct(argv[1]); //vriskw poses synolika grammes exei to arxeio
	
	//mikra kommatia kwdika exoun xrhsimopoiithei apo to ergasthrio tou mathimatos apo ta arxeia pou vriskontai sto eclass
	if ((shmid = shmget(IPC_PRIVATE, SHMSIZE,0666)) < 0){ //diamoirazomeni mnhmh theloume na exe megethos shmsize
		perror("shmget error");
		exit(1);
	}
	printf("Shared memory has ID: %d\n", shmid);

	int *pos=(int*) shmat(shmid,(void*)0, 0); //segment of shared mem attachment
	//pleon to pos deixnei sthn prwth thesi ths shared meme
	// if ((int) pos == -1) {
	// 	perror("attachment error");
	// 	exit (2);
	// }

	//dhmiourgia kai arxikopoihsh twn semaforwn mas
	sem_t *read=(sem_t *)(pos+sizeof(int));
	int retval1 = sem_init(read, 1, 0);
	if (retval1 != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

	sem_t *write=(sem_t *)(pos+sizeof(int)+sizeof(sem_t));
	int retval2 = sem_init(write, 1, 0);
	if (retval2 != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

	sem_t *unique=(sem_t *)(pos+sizeof(int)+2*sizeof(sem_t));
	int retval3 = sem_init(unique, 1, 1); 	//thes prwti fora na mpei to paidi
	if (retval3 != 0){
		perror("Couldn't initialize.");
		exit(3);
	}
	//pid_t fatherpid=getpid();
	pid_t pid,wpid;
	for(int j=0;j<k;j++){
		pid=fork();
		if(pid==0){	//an einai to paidi vgainei, alliws pateras ftiaxnei ola ta paidia
			break;
		}
	}
	if(pid==0){	//paidi
        srand(getpid()); ////////////////////////////
        //N FORES (aithmata)
        double sumtime=0,mean,xron;
        for (int i=0;i<n;i++){
			//printf("eimai to paidi\n");
			sem_wait(unique);	//den afhnw allo child process na vgei
			int temp=(rand()%lines)+1;
			*pos=temp;	//prwth thesi shared memory deixnei se integer poy tha xrhsimopoiithei gia na deiksei se tyxaia grammh tou arxeiou
			cl=clock(); //arxizei to roloi efoson molis egine to aithma
			printf("%d requested %d\n",getpid(),*pos);
			sem_post(read);	//enhmerwnw patera oti exw grapsei
			sem_wait(write);	//perimenei paidi na grapsei o pateras sth mnhmh
			cl=clock()-cl; //telos metrimatos efoson paidi elave to aitima apton patera
			xron=((double)cl)/CLOCKS_PER_SEC;
			sumtime=sumtime+xron;
			printf("%d received %s\n",getpid(),(char*)(pos+sizeof(int)+3*sizeof(sem_t)));  //kanw print thn rand grammh toy arxeiou
			sem_post(unique); //afhnw allo paidi na mpei
		}
		mean=sumtime/((double)n);	//to n einai integer ara to kanw cast se double gia na einai dynath h diairesh
		printf("the mean time it took for dosolipsies to run was:%f \n",mean);
		shmdt(pos);
		printf("Detachment of shared memory region (child)\n\n");
		return 1;
	}
	else if(pid>0){	//pateras
         //k paidia, n aithmata to kathena
        for(int i = 0; i < k; i++){
        	for(int j=0;j<n;j++){
	            sem_wait(read); 	//perimenei gia enhmerwsi paidioy
	            int ait=*pos;	//diavazw ton akeraio pou exei zitisei to paidi
	            funct2(argv[1],ait,pos); //synarthsh vriskei th protash sth grammh pou zhthse to paidi kai to grafei sth mnhmh
	            sem_post(write); //enhmerwnw to paidi pws h grammh graftike sto shared mem
	        }
        }
        while((wpid=wait(&status)>0)); //perimenw mexri na teleiwsoun ola ta paidia
	}

	shmdt(pos);	/*kanw detach to shared memory region*/
	printf("Detachment of shared memory region (parent)\n");

	/* Remove segment. */
	int err = shmctl(shmid, IPC_RMID, 0);
	if (err == -1) {
		perror("Removal error.");
	} else {
		printf("Segment removed. %d\n",err);
	}
	return 2;
}
