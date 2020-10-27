#include<iostream>
#include<cstdlib>        /*exit() */
#include<unistd.h>      /*fork()*/
#include<sys/wait.h>  /* wait()*/

using namespace std;

int B=10;


int main(void)
{
 int retval,i;

 B =B+10; 

 retval=fork();
 
 if(retval==0){
   B +=1;
   exit(0); //Kraj procesa djeteta
  }
 else if(retval>0){
  B +=2;
  wait(NULL);
 }
 else
   cout<<"Pogreska"<<endl;

 cout<<B<<endl;
 return 0; //Kraj procesa roditelja
}
