#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int some=0;
pthread_mutex_t lock;


void *route(void* arg){
  char* thread_name=(char*)arg;
  while(1)
  {
	  pthread_mutex_lock(&lock);

	  cout<<thread_name << some<<endl;
    some++;
    usleep(1000);
    sched_yield(); 
	  pthread_mutex_unlock(&lock);
  }
}
int main(){
  pthread_t t1,t2,t3;

  pthread_mutex_init(&lock, NULL);//动态分配

  pthread_create(&t1, NULL, route, (void*)"thread 1 ");
  pthread_create(&t2, NULL, route, (void*)"thread 2 ");
  pthread_create(&t3, NULL, route, (void*)"thread 3 ");
  
//   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
//   pthread_join(t3, NULL);
  
  pthread_mutex_destroy(&lock);
}


