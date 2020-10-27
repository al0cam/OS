#include<iostream>
#include<cstdlib>        /*exit() */
#include<unistd.h>      /*fork()*/
#include<sys/wait.h>  /* wait()*/

using namespace std;

int main(void)
{
 int retval,i;
 
 retval=fork();
 
 if(retval==0){
   for(i=0;i<10;i++){
      cout<<"Dijete i="<<i<<endl;
      usleep(70000);
  }
   exit(0); //Kraj procesa djeteta
  }
 else if(retval>0){
  for(i=0;i<10;i++){
      cout<<"Roditelj i="<<i<<endl;
      usleep(100000);
  }
  wait(NULL);
 }
 else
   cout<<"Pogreska"<<endl;

 return 0; //Kraj procesa roditelja
}
