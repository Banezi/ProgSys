#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *function(void* args)
{
    printf("%d\n", *(int*) args);
}
void main()
{
    int numbers[] = {1,2,3,4};
    for(int i=0; i<4; i++)
    {
        pthread_t thread;
        pthread_create(&thread,0,function,numbers+i);
    }
    sleep(2);
}