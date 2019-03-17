#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

pid_t fork(void);
int main()
{
  pid_t pid;
  if((pid=fork())==0){
     int res = execl("/usr/bin/vi","vi",NULL);

     if(res == -1)
     perror("execl");
  }
  while(1){}
  return 0;
}
