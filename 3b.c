#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

int pthread_yield(void);
volatile int ticketCount = 1000;

sem_t *mySem = NULL;
//read
void *worker1(void *arg)
{
    int temp = 0;
    sem_wait(mySem);
    temp = ticketCount;
    pthread_yield();
    temp = temp - 1;
    pthread_yield();
    ticketCount = temp;
    sem_post(mySem);
    return NULL;
}

//print
void *worker2(void *arg)
{
    int temp = 0;
    sem_wait(mySem);
    temp=ticketCount;
    pthread_yield();
    temp=temp+1;
    pthread_yield();
    ticketCount=temp;
    sem_post(mySem);
    return NULL;
}


int main(int argc, char *argv[])
{
    // loops = atoi(argv[1]);
    if(argc!=3)
	{
		printf("请正确输入参数！\n");
		exit(0);
	}
    int i;
    printf("初始票数为：%d\n",ticketCount);
    pthread_t p[1000];
    //printf("Initial value : %d\n", counter);

    mySem = sem_open("myname", O_CREAT, 0666, 1);
    for(i=0;i<atoi(argv[1]);i++)
    {
        pthread_create(&p[i], NULL, worker1, NULL);
    }
    for(i=500;i<atoi(argv[2])+500;i++)
    {
        pthread_create(&p[i], NULL, worker2, NULL);
    }
    for(i=0;i<atoi(argv[1]);i++)
    {
        pthread_join(p[i], NULL);
    }
    for(i=500;i<atoi(argv[2])+500;i++)
    {
        pthread_join(p[i], NULL);
    }
    sem_close(mySem);
    printf("the number of tickets are %d\n",ticketCount);

    sem_unlink("myname");
    return 0;
}
