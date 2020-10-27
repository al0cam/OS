#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <semaphore.h>
using namespace std;

int br,m; 
pthread_mutex_t monitor;
pthread_cond_t uvjet[2];  

int tr, zvr, ceka[2]; 
int u_restoranu=-1; 

void ispis(int vr, string tx)
{
    cout<<"Red Linuxasa:"; 
    for(int i=0;i<m;i++)
        if(i<ceka[0]) 		
            cout<<"L"; 
        else
            cout<<"-"; 

    cout<<"Red Microsoftasa:"; 
    for(int i=0;i<m;i++)
        if(i<ceka[1])
            cout<<"M";
        else
            cout<<"-";
    cout<<"Restoran:";
    if(u_restoranu==1) 
        for(int i=0;i<tr;i++)
            cout<<"M";
    else 
        for(int i=0;i<tr;i++)
            cout<<"L";
    cout<<"-->";
    if(vr==1)
        cout<<"M "<<tx;
    else
        cout<<"L "<<tx;
    cout<<endl;
}

void udzi(int &vr) 
{
    pthread_mutex_lock(&monitor); 
    ceka[vr]++;  

    while(u_restoranu==1-vr || zvr>=br) 
    {
        ispis(vr, "u red cekanja"); 
        pthread_cond_wait(&uvjet[vr], &monitor); 
    }           

    ceka[vr]--; 
    u_restoranu=vr; 
    tr++; 
    if(ceka[1-vr]>0) 
        zvr++;  
    ispis(vr," u restoran"); 
    pthread_mutex_unlock(&monitor); 
}

void izadzi(int &vr) 
{
    pthread_mutex_lock(&monitor); 
    tr--; 
    if(tr==0) 
    {
        if(ceka[1-vr]>0) 
        {
            u_restoranu=1-vr; 
            pthread_cond_broadcast(&uvjet[1-vr]); 
        }

        else 
        {
            u_restoranu=-1;
        }
        zvr=0; 
    }
    ispis(vr,"iz restorana");
    pthread_mutex_unlock(&monitor);
}

void *programer(void *arg)
{
    int vr;
    usleep(rand()%100000); 
    vr=*((int *)arg);
    udzi(vr);
    usleep(rand()%100000); 
    izadzi(vr);
}

void brisi(int sig)
{
    pthread_mutex_destroy(&monitor); 
    for(int i=0;i<2;i++)
        pthread_cond_destroy(&uvjet[i]);   
    exit(0);
}

int main(int argc, char *argv[])
{ 	if(argc != 3){
 		 cout << "Mora biti 2 arg\n";
 		 exit(0);
 	}
    br=atoi(argv[1]);
    m=atoi(argv[2]);
    sigset(SIGINT,brisi);
    srand(time(0));
    pthread_mutex_init(&monitor, NULL); 
    for(int i=0;i<2;i++)
        pthread_cond_init(&uvjet[i],NULL); 

    pthread_t id[m*2]; 
    int polje[m*2]; 
    for(int i=0;i<m*2;i++) 
        polje[i]=i%2; 

    for(int i=0;i<m*2;i++)
        pthread_create(&id[i],NULL,&programer, &polje[i]);
    for(int i=0;i<m*2;i++)
        pthread_join(id[i],NULL);

    brisi(1);
return 0;
}
