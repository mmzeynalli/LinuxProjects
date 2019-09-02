#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <limits.h>


#define CHILDREN 10

struct rusage res;
int st, op;
int i;
int a = 0;
pid_t pid = 0, cpid;
double t;

int main()
{

  printf("Parent: %d\n", getpid());

  //if((pid = fork()) < 0) exit(1);

  for(i = 0; i < CHILDREN; i++)
  {
    pid = fork();
    
    if (pid == 0) {
      for (long long j=0; j < 10000000; j++);
//	printf("%d\n", getpid());
     // for (long long j=0; j < LLONG_MAX; j++);	
     // do something
      exit(0);
    }

	
  }

  printf("id = %d, parent = %d\n", getpid(), getppid());

  for(i = 0; i < CHILDREN; i++)
  {
    
     cpid = wait4(-1, &st, 0, &res);

 
     t = res.ru_utime.tv_sec + (res.ru_utime.tv_usec / 1000000.0);
		printf("%lf\n", t);
 
  } 


  exit(0);
}
