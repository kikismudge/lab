#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



#include<sys/types.h>
#include<pthread.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <semaphore.h>    





int main(){

pid_t p1,p2,p3,p4;

sem_t* flag1= NULL;
sem_t* flag2 = NULL;
sem_t* mutex = NULL;

flag1 = sem_open("P2", O_CREAT, 0666, 0);
flag2 = sem_open("P3", O_CREAT, 0666, 0);
mutex = sem_open("mutex", O_CREAT, 0666, 0);

p1 = fork();
if(p1 > 0){
printf("I am the process P1\n");
sem_post(mutex);
sleep(1);
while((p3=fork())==-1);
if(p3>0)sleep(1);
else if(p3 == 0){
sem_wait(mutex);
printf("I am the process P3\n");
sem_post(mutex);
sem_post(flag2);
}
}
else if(p1 == 0){
while((p2=fork())==-1);
if(p2>0){
sem_wait(mutex);
printf("I am the process P2\n");
sem_post(mutex);
sem_post(flag1);
sleep(1);
while((p4=fork())==-1);
if(p4>0)sleep(1);
else if(p4 == 0){
sem_wait(flag1);
sem_wait(flag2);
printf("I am the process P4\n");
}}
}


sem_close(flag1);
sem_close(flag2);
sem_close(mutex);
sem_unlink("P2");
sem_unlink("P3");
sem_unlink("mutex");






return 0;
}
