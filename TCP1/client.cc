#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

using namespace  std;

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    cerr<<"Usage: "<<argv[0]<<" ip port"<<endl;
    return 1;
  }

  //创建套接字
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock<0)
  {
    cerr<<"socket error"<<endl;
    exit(2);
  }
  struct sockaddr_in server;
  bzero(&server,sizeof(server));
  socklen_t len = sizeof(server);

  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  server.sin_addr.s_addr = inet_addr(argv[1]);

  int check = connect(sock,(struct sockaddr*)&server,len);
  if(check<0)
  {
    cerr<<"connect error"<<endl;
    exit(3);
  }

  string message;
  char buf[1024];

  while(1)
  {
    cout<<"Please echo# ";
    cin>>message;
    write(sock,message.c_str(),message.size());
    ssize_t s = read(sock,buf,sizeof(buf)-1);
    if(s>0)
    {
      buf[s]=0;
      cout<<"Server echo# "<<buf<<endl;
    }
  }
  close(sock);
  return 0;
}
