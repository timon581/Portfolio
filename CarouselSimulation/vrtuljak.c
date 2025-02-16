#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

sem_t *vrtuljak;
sem_t *sjeo;
sem_t *kraj;
sem_t *sisao;
#define N 5

int main(void) {

	int id1 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
	vrtuljak = shmat (id1, NULL, 0);
	shmctl(id1, IPC_RMID ,NULL); 
	
	int id2 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
	sjeo = shmat (id2, NULL, 0);
	shmctl(id2, IPC_RMID ,NULL); 
	
	int id3 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
	kraj = shmat (id3, NULL, 0);
	shmctl(id3, IPC_RMID ,NULL); 
	
	int id4 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
	sisao = shmat (id4, NULL, 0);
	shmctl(id4, IPC_RMID ,NULL); 
	
	sem_init(vrtuljak, 1, 0);
	sem_init(sjeo, 1, 0);
	sem_init(kraj, 1, 0);
	sem_init(sisao, 1, 0);
	
	if(fork() == 0){
		//posjetitelji
		while(1){
			
			for(int i = 0; i < N; i++){
				sem_wait(vrtuljak);
				printf("Posjetitelj sjeo\n");
				sleep(1);
			} 
			
			for(int i = 0; i < N; i++) sem_post(sjeo);
			for(int i = 0; i < N; i++){
				sem_wait(kraj);
				printf("Posjetitelj sisao\n");
				sleep(1);
			} 
			
			for(int i = 0; i < N; i++) sem_post(sisao);
		}
		
	}
	if(fork() == 0){
		//vrtuljak
		while(1){
		       
			for(int i = 0; i < N; i++) sem_post(vrtuljak);
			printf("Ukrcavanje u vrtuljak!\n");
			for(int i = 0; i < N; i++) sem_wait(sjeo);
			printf("Pokrecem vrtuljak!\n");
			sleep(1); 
			printf("Zaustavljam vrtuljak!\n"); 
			sleep(1);
			for(int i = 0; i < N; i++) sem_post(kraj);
			for(int i = 0; i < N; i++) sem_wait(sisao);
		
		}
		
	}
	(void) wait(NULL);
	(void) wait(NULL);
	
	sem_destroy(vrtuljak);
	shmdt(vrtuljak);
	sem_destroy(sjeo);
	shmdt(sjeo);
	sem_destroy(kraj);
	shmdt(kraj);
	sem_destroy(sisao);
	shmdt(sisao);
	return 0;
}
