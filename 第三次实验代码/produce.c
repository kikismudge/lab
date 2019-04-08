#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



#include<sys/types.h>
#include<pthread.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <semaphore.h>




sem_t* scanf_sem = NULL;
sem_t* print_sem = NULL;
   
char buf[10];
int i,j = 0;

   
void*scanf_char(){
    while(1){
        sem_wait(scanf_sem);
        char *t = &buf[i++%10];
        scanf("%c",t);
        sem_post(print_sem);
        }
}
   
void*print_char(){
    while(1){
        sleep(1); 
        sem_wait(print_sem);
        char *t = &buf[j++%10];  
        printf("%d:%c\n",j,*t);
        sem_post(scanf_sem);
    }
}
   
int main(){
    pthread_t p1,p2;
   
    scanf_sem = sem_open("scanf", O_CREAT, 0666, 10);
    print_sem = sem_open("print", O_CREAT, 0666, 0);
   
    pthread_create(&p1,NULL,scanf_char,NULL);
    pthread_create(&p2,NULL,print_char,NULL);
   
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
   
    sem_close(scanf_sem);
    sem_close(print_sem);
   
    sem_unlink("scanf");
    sem_unlink("print");
    return 0;
}
