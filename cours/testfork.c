#include <stdio.h>
#include <unistd.h>

void main()
{
    fork();
    fork();
    printf("Je suis %d\n", getpid());
}