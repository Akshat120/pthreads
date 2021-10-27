#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
int rc=0;
int value=100;
pthread_mutex_t db_lock,mutex;
void* read(void *tid)
{
    pthread_mutex_lock(&mutex);
    rc=rc+1;
    if(rc==1) pthread_mutex_lock(&db_lock);
    pthread_mutex_unlock(&mutex);
    printf("Read: %d\n",value);
    pthread_mutex_lock(&mutex);   
    rc=rc-1;
    if(rc==0) pthread_mutex_unlock(&db_lock);
    pthread_mutex_unlock(&mutex); 
}
void* write(void *tid)
{
    pthread_mutex_lock(&db_lock);
    int change = rand()%100;
    value=value+change;
    printf("Write: %d (+%d)\n",value,change);
    pthread_mutex_unlock(&db_lock);
}
int main()
{
    srand(time(0));
    int r=2,w=2;
    pthread_mutex_init( &db_lock, NULL);
    pthread_mutex_init( &mutex, NULL);
    pthread_t readers[r],writers[w];
    for(int i=0;i<r;i++)
    {
        pthread_create(&readers[i],NULL,read,(void *)&readers[i]);
    }
    for(int i=0;i<w;i++)
    {
        pthread_create(&writers[i],NULL,write,(void *)&writers[i]);
    }    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&db_lock);
    pthread_exit(NULL);
}
