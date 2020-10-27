#include<iostream>
#include<string>
#include<unistd.h>
#include<signal.h>

using namespace std;

void prekidna(int signum)
{
    int i=0;
  string ime_signala;
  switch(signum)
  {
   case SIGTSTP:
    ime_signala="SIGTSTP ";
    break;
   case SIGQUIT:
    ime_signala="SIGQUIT ";
    break;
   default:
    ime_signala="? ";
  }

  while(i<5){
    i++;
    sleep(1);
    cout<<ime_signala<<i<<"/5"<<endl;
  }
}

void maskiraj(int signum)
{
    sigset(signum,prekidna);
}

int main()
{
 int i=0;
 maskiraj(SIGTSTP);
 maskiraj(SIGQUIT);
 while(i<20)
 {
  i++;
  sleep(1);
  cout<<"Glavni "<<i<<"/20"<<endl;
 }
 return 0;
}