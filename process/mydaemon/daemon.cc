#include <signal.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
using namespace std;
int main_ (const char* argv1,const char* argv2 )
{
int fd = socket(AF_INET, SOCK_STREAM, 0);
if (fd < 0) {
  perror("socket error");
  return 1;}

struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr(argv1 );
addr.sin_port = htons(atoi(argv2));
int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
if (ret < 0) {
  perror("bind error ");
  return 1;

}
ret = listen(fd, 10);
if (ret < 0) {
  perror("listen error ");
  return 1;

}
for (;;) 
{
  
  cout<<"http server open...\n"<<endl;
  cout<<"下面会打印首行(first_line,头部(header),空格(\r\n),正文(body))"<<endl;
  struct sockaddr_in client_addr;
  socklen_t len;
  int client_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
  if (client_fd < 0) {
    perror("accept");
    continue;
  }
  char input_buf[1024 * 10] = {0}; // 用一个足够大的缓冲区直接把数据读完.
  ssize_t read_size = read(client_fd, input_buf, sizeof(input_buf) - 1);
  if (read_size < 0) {
    return 1;
  }
  printf("[Request-------------->] %s", input_buf);
  char buf[1024] = {0};
  const char* hello ="<a href=\"https://github.com/Chinaliuxin\" title=\"open_to liuxin's github\"target=\"_blank\">click the mouse to github</a><a></a>";
  sprintf(buf, "HTTP/1.1 200 OK\nContent-length:%lu\n\n%s", strlen(hello), hello);
  write(client_fd,buf,strlen(buf));
  close(client_fd);
  
  //same to    send(client_fd, buf, strlen(buf),0);
}return 0;
}

int main(int argc,char**argv)
{
  if(argc!=3){cout<<"input ip and port"<<endl;return 0;}
  pid_t pc=fork();
  if(pc<0)
  {
    perror("fork error \n");
    exit(1);
  }
  if(pc>0){exit(0);}

  umask(0);//quanxian   4next
  setsid();//2 next
  chdir("/");//  to  root chdir  3next
  umask(0);//quanxian   4next
  for(size_t i=0;i<65535;i++)
    close(i);
  signal(SIGCHLD,SIG_IGN);
  main_(argv[1],argv[2]);
  return 0;
}
