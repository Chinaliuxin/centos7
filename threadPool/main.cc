#include "threadpool.hpp"
//线程池中线程的数目
const int thread_num = 5;
//向任务队列中添加几个简单任务
int Add(int x, int y)
{
  cout<<"刘昕"<<endl; 
  return x + y;
}
int Sub(int x, int y)
{
  return x - y;
}
int Div(int x, int y)
{
  return x/y;
}
int Mul(int x, int y)
{
  return x * y;
}

int main()
{
  ThreadPool* tp = new ThreadPool(thread_num);
  tp -> InitThread();//初始化线程池

  int count = 1;
  while(1)
  {
    sleep(1);
    Task t(count, count - 1, Add);
    tp -> AddTask(t);
    Task t1(count, count - 1,Sub);
    tp -> AddTask(t1);
    Task t2(count, count, Mul);
    tp -> AddTask(t2);
    Task t3(count, count, Div);
    tp -> AddTask(t3);
    ++count;
  }
  return 0;
}
