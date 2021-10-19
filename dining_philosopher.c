#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
int count;
pthread_mutex_t *forks;

void* eatFood(void *id)
{
    int cnt = *(int *)(id);
    if(cnt<count-1)
    {
        pthread_mutex_lock(&forks[cnt]);    
        pthread_mutex_lock(&forks[(cnt+1)%count]);
        printf("(philosopher %d): %d\n",cnt,rand()%100);
        pthread_mutex_unlock(&forks[cnt]);    
        pthread_mutex_unlock(&forks[(cnt+1)%count]);       
    }
    else
    {
        pthread_mutex_lock(&forks[(cnt+1)%count]);    
        pthread_mutex_lock(&forks[cnt]);
        printf("(philosopher %d): %d\n",cnt,rand()%100);
        pthread_mutex_unlock(&forks[(cnt+1)%count]);    
        pthread_mutex_unlock(&forks[cnt]);  
    }
}
int main()
{
    srand(time(0));
    count=5;
    forks = (pthread_mutex_t *)malloc(count * sizeof(pthread_mutex_t));
    for(int i=0;i<count;i++) 
    {
        pthread_mutex_init( &(forks[i]), NULL);
    }
    
    pthread_t philosopher[count];
    int id[count];

    for(int i=0;i<count;i++) id[i]=i;
    
    for(int i=0;i<count;i++)
    {
        pthread_create(&philosopher[i],NULL,eatFood,(void *)&id[i]);
    }

    for(int i=0;i<count;i++) pthread_mutex_destroy(&forks[i]);
    
    pthread_exit(NULL);
}