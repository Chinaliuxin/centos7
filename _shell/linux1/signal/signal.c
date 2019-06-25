#include<fcntl.h>
#include <signal.h>
#include<stdio.h>
#include<unistd.h>

//delete
static void handler_d (int sig,siginfo_t *si, void* data)
{
  int event_fd,event_pid;
  event_fd=si->si_fd;
  event_pid=si->si_pid;
  printf("process:%d ,has deleted something from fd %d:\n",event_pid,event_fd);
}

static void handler_m(int sig,siginfo_t* si,void* data)
{
  
  int event_fd,event_pid;
  event_fd=si->si_fd;
  event_pid=si->si_pid;
  printf("process:%d ,has motified  something of fd %d:\n",event_pid,event_fd);
}

int main(void )
{
  struct sigaction act_d,act_m;
  int fd1,fd2;
  pid_t pid;

  if((pid =fork())<0)
  {
    printf("fork error");
    exit(1);
  }
  else if (pid ==0)
  {
    if((fd1=open("/home/rensg",O_RDONLY))==-1)
    {
      printf("open /home /rensg error!\n");
    }
    if((fd2=open("/home/jiang",O_RDONLY))==-1)
    {
      printf("open /home /jiang error \n" );

    }



    act_m.sa_sigaction=handler_m;
    sigemptyset(&act_m.sa_mask );
    act_m.sa_flags=SA_SIGINFO;


    sigaction(SIGRTMIN+2,&act_m,NULL);


    fcntl(fd1,F_SETSIG,SIGRTMIN+2);
    fcntl(fd1,F_NOTIFY,DN_MODIFY|DN_MULTISHOT);

    fcntl(fd2,F_SETSIG,SIGRTMIN+2);
    fcntl(fd2,F_NOTIFY,DN_MODIFY|DN_MULTISHOT);


    while(1)
    {
      pause();

    }

}


else //parent
{

    if((fd1=open("/home/rensg",O_RDONLY))==-1)
    {
      printf("open /home /rensg error!\n");
    }
    if((fd2 =open("/home/jiang",O_RDONLY))==-1)
    {
      printf("open /home /jiang error \n" );
    }


    printf(".home /rensg,fd=%d\n"fd1 );
    printf(".home /jiang,fd=%d\n"fd2 );

    act_d.sa_sigaction=handler_d;
    sigemptyset(&act_d.sa_mask);
    act_d.sa_flags=SA_SIGINFO;


    sigaction(SIGRTMIN+1,&act_d,NULL);
    fcntl(fd1,F_SETSIG,SIGTMIN+1);
    fcntl(fd1,F_NOTIFY,DN_DELETE|DN_MULTISHOT);


    fcntl(fd2,F_SETSIG,SIGTMIN+1);
    fcntl(fd2,F_NOTIFY,DN_DELETE|DN_MULTISHOT);

    while(1)
    {
      pause();
    }


}
