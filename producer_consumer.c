#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int items=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* produce(void *tid)
{ 
  pthread_mutex_lock(&lock);
  items++;
  printf("(producer) Items: %d\n",items);
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock);   
}
void* consume(void *tid)
{
  pthread_mutex_lock(&lock);
  while(items==0)
  {
      pthread_cond_wait(&cond,&lock);
  }    
  items--;
  printf("(consumer) Items: %d\n",items);
  pthread_mutex_unlock(&lock);
}
int main(){
    int p=5,c=5;
    pthread_t producer[p],consumer[c];
    
    for(int i=0;i<p;i++)
    {
        pthread_create(&producer[i],NULL,produce,(void *)&producer[i]);
    }

    for(int i=0;i<c;i++)
    {
        pthread_create(&consumer[i],NULL,consume,(void *)&consumer[i]);
    }

    pthread_exit(NULL);
    return 0;
}