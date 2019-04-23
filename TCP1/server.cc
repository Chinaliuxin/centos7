#include "server.hpp"

int main(int argc,char* argv[])
{
  if(argc != 2)
  {
    cerr<<"Usage: "<<argv[0]<<" port"<<endl;
    return 1;
  }
  //string ip = argv[1];
  int port = atoi(argv[2]);

  Server* s = new Server(port);
  s->InitServer();
  s->Run();

  delete s;
  return 0;
}
