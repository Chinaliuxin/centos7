#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
void mydaemon()
{
    umask(0);
    if(fork() <=  0)//child
    {}
    else//father
    {
        exit(0);
    }

    setsid();
    signal(SIGHUP,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);
    close(0);
  //close(1);
    close(2);
    while(1){printf("123\n");}
    chdir("/");



}



int main()
{
    mydaemon();
//  daemon(0,0);
    while(1);
    return 0;
}
