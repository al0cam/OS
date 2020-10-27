#include<iostream>
#include<cstdlib>        /*exit() */
#include<unistd.h>      /*fork()*/
#include<sys/wait.h>  /* wait()*/

using namespace std;

void Dijete(int j)
{
 for(int i=0;i<5;i++){
      cout<<"P("<<j<<") -> "<<i<<endl;
      usleep(10000);
   }
 exit(0); //Kraj procesa djeteta
}


int main(void)
{
 int retval,i;

 for(int j=1;j<=4;j++) 
 switch(fork())
 {
  case -1:
   cerr<<"Pogreska"<<endl;
   exit(-1);
   break;
  case 0:
   Dijete(j);
   break;
 default:
   break;
 }

 wait(NULL);
 return 0; //Kraj procesa roditelja
}
