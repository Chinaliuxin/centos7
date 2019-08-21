#include <iostream>
#include<unistd.h>
#include<pthread.h>
using  namespace std;
typedef struct ARG{
  int a;
  char ch ;
}arg;
static int id =0; 
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond=PTHREAD_COND_INITIALIZER; 
void* fun(void* arg1)

{
//   for(int i=0;i<3;i++){
   pthread_mutex_lock(&lock);
   arg* ar=static_cast<arg *>(arg1);
   while (id ==ar ->ch-'a'){
     pthread_cond_wait(&cond,&lock);
   }
   id=(id+1)%26;
    cout<<ar->ch ;//<<endl;
  //sleep(1);
 
   // cout<<static_cast<arg*>(arg1)->ch; 
  pthread_mutex_unlock(&lock);
  
 pthread_cond_broadcast(&cond );
 // cout<<endl;
 // }
}

int main()
{
 // cout<<'d';
  int n;
  cin>>n;
  pthread_t thread[n ];
  for(int i=0;i<n;i++){
    arg*aa=new arg;
    aa ->a=n ;
    aa->ch=i+'a';
  pthread_create(&thread[i ],NULL,fun,(void*)aa);}
  for(int i=0;i<n ;i++)
  {
    pthread_join(thread[i],NULL);
  }

}


