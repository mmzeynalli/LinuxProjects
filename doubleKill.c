#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void killHandler(int signo);
int cnt;


int main()
{
  //SIGQUIT -> ctrl + \
  //SIGINT -> ctrl + c
  //SIGSTOP -> ctrl + z

  s.sa_handler = killHandler;
  s.sa_flags = 0; //reset flag
  sigemptyset(&s.sa_mask); //empty mask
  
  sigaction(SIGINT, &s, NULL);
  
  alarm(2);

  while(1);
  
}

void killHandler(int signo)
{
  if(signo == SIGINT)
    if(++cnt == 2)
      exit(0);
}
