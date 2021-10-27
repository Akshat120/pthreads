#define MAX 30
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *client(void *param);
void *barber(void *param);

sem_t chairs_mutex;
sem_t sem_client;
sem_t sem_barber;
int num_chairs;
int clientWait;
int main(int argc, char *argv[]) {
	pthread_t barberid;
	pthread_t clientids[MAX];
	
	printf("Main thread beginning\n");
   int runTime,clients,i;
   if (argc != 5){
	   printf("Please enter 4 arguments: <Program run time> <Number of clients>\n");
	   printf("<Number of chairs> <Client wait time>\n");
	   exit(0);
   }
   runTime = atoi(argv[1]);
   clients = atoi(argv[2]);
   num_chairs = atoi(argv[3]);
   clientWait = atoi(argv[4]);
   
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
   
   pthread_create(&barberid, NULL, barber, NULL);
   printf("Creating barber thread with id %lu\n",barberid);
   
   for (i = 0; i < clients; i++){
	   pthread_create(&clientids[i], NULL, client, NULL);
	   printf("Creating client thread with id %lu\n",clientids[i]);
   }
   
   printf("Main thread sleeping for %d seconds\n", runTime);
   sleep(runTime);
   
   printf("Main thread exiting\n");
   exit(0);
}

void *barber(void *param) {
   int worktime;
  
   while(1) {
	  sem_wait(&sem_client);
	  sem_wait(&chairs_mutex);
	  num_chairs += 1;
	  printf("Barber: Taking a client. Number of chairs available = %d\n",num_chairs);
	  sem_post(&sem_barber);
	  sem_post(&chairs_mutex);
	  worktime = (rand() % 4) + 1;
	  printf("Barber: Cutting hair for %d seconds\n", worktime);
	  sleep(worktime);
    } 
}

void *client(void *param) {
   int waittime;

   while(1) {
	  sem_wait(&chairs_mutex);
      
	  if(num_chairs <= 0){
		   printf("Client: Thread %u leaving with no haircut\n", (unsigned int)pthread_self());
		   sem_post(&chairs_mutex);
	  }
	  else
      {
		   num_chairs -= 1;
		   printf("Client: Thread %u Sitting to wait. Number of chairs left = %d\n",(unsigned int)pthread_self(),num_chairs);
		   sem_post(&sem_client);
		   sem_post(&chairs_mutex);
		   sem_wait(&sem_barber);
		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
	  }
      Max value from command line. */
	  waittime = (rand() % clientWait) + 1;
	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n",(unsigned int)pthread_self(),waittime);
	  sleep(waittime);
     }
}

