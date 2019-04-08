#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



#include<sys/types.h>
#include<pthread.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <semaphore.h>    

int train_tickets=1000;
sem_t* mySem = NULL;


void*sell(){
    int i = 2000000 ;
    while(i--){
         sem_wait(mySem);        
         train_tickets--;
        // sched_yield();         
       
         sem_post(mySem);     
}
}

void*refund(){
    int i = 2000000;
    while(i--){
       sem_wait(mySem);         
       train_tickets++;
        // sched_yield();         
              
       sem_post(mySem);     
}
}

int main(){

    pthread_t p1,p2;
    mySem = sem_open("train_ticket", O_CREAT, 0666, 1);
    pthread_create(&p1,NULL,sell,NULL);
    pthread_create(&p2,NULL,refund,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    sem_close(mySem);
    sem_unlink("train_ticket");
    printf("The number of train tickets is %d.\n",train_tickets);



    return 0;
}


