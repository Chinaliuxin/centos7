#include<iostream>
#include<pthread.h>

using namespace std;

pthread_mutex_t  lock;
pthread_cond_t  cond;
size_t id=0;
typedef struct ARG{
char ch;
size_t n;

}myarg;

void* fun(void* arg1)

{
  myarg* arg=static_cast<myarg*>(arg1 );
  for(size_t i=0;i<3;i++)
  {
    pthread_mutex_lock (&lock);
     cout<<arg->ch;

    while(id==(arg->ch-'a'))
    {
      pthread_cond_wait(&cond,&lock);
    
    }
    id=(id+1)%26;
    if(arg->ch-'a'<arg->n)cout<<arg->ch;

  

    pthread_mutex_unlock(&lock);
     pthread_cond_broadcast(&cond);
    
  } 
  //cout<<arg->ch;
}
int main()
{
  pthread_t thread[26];
  int n=5;
  //cin>>n; 
  // pthread_cond_init(&cond,NULL);
  // pthread_mutex_init(&lock,NULL);
  for(size_t i=0;i<n;i++)
  {
    myarg* arg1=new myarg ;
    arg1->n=n;
    arg1->ch=i+'a';
    pthread_create(&thread[i],NULL,fun,static_cast<void*> (arg1));
  }

  for(size_t i=0;i<n;i++)
  {
   pthread_join(thread[i],NULL);
  }

   pthread_mutex_destroy(&lock);
   pthread_cond_destroy(&cond);
return 0;
}

