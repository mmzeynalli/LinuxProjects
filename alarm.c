#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

struct sigaction s;

void sigHandler(int signo);

int main()
{
  //printf("HELLO\n");
  
  s.sa_handler = sigHandler;
  s.sa_flags = 0; //reset flag
  sigemptyset(&s.sa_mask); //empty mask
  
  sigaction(SIGALRM, &s, NULL);
  
  alarm(2);

  while(1); //do not let main program to finish

  exit(0);
}

void sigHandler(int signo) 
{
  printf("HELLO\n");
  
  alarm(2);

}

