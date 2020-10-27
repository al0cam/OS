/****** pokreni.cpp  *****/
#include<unistd.h>
#include<iostream>

using namespace std;

int main(int argc,char **argv)
{
 if(argc==2)
   execl(argv[1],(char*)0);
 else
   cout<<"Pogreska!"<<endl;
 return 0;
}

