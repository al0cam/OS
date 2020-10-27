#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sys/shm.h>
#include <cstring>
using namespace std;
struct shared
{
    int tr, zvr, ceka[2]; 
    int u_restoranu=-1; 
    pthread_mutex_t monitor;
    pthread_cond_t uvjet[2]; 
};

int id_podjeljeno; 
int mjesta,ljudi;
void postavi(int mjesta_main, int ljudi_main)
{
    mjesta=mjesta_main;
    ljudi=ljudi_main;
}

shared *s;

void ljudi_u_redu(string vrsta,string slovo)
{
    cout<<vrsta; 
    for(int i=0;i<ljudi;i++)
        if(i<(*s).ceka[0]) 		
            cout<<slovo; 
        else
            cout<<"-"; 
}
void restoran(int vr,string tx)
{
    cout<<"Restoran:";

    if((*s).u_restoranu==1) 
        for(int i=0;i<(*s).tr;i++)
            cout<<"M";
    else 
        for(int i=0;i<(*s).tr;i++)
            cout<<"L";
    cout<<"-->";

    if(vr==1)
        cout<<"M "<<tx;
    else
        cout<<"L "<<tx;
    cout<<endl;
}
void ispis(int vr, string tx)
{
    ljudi_u_redu("Red Linuxasa:","L");
    ljudi_u_redu("Red Microsofva:","M");
    restoran(vr,tx);
}

void udzi(int &vr) 
{
    pthread_mutex_lock(&(*s).monitor); 
    (*s).ceka[vr]++;  

    while((*s).u_restoranu==1-vr || (*s).zvr>=mjesta) 
    {
        ispis(vr, "u redu cekanja"); 
        pthread_cond_wait(&(*s).uvjet[vr], &(*s).monitor); 
    }           

    (*s).tr++; 
    (*s).u_restoranu=vr; 
    (*s).ceka[vr]--; 
    if((*s).ceka[1-vr]>0) (*s).zvr++;  
    ispis(vr," u restoranu"); 
    pthread_mutex_unlock(&(*s).monitor); 
}
void izadzi(int &vr) 
{
    pthread_mutex_lock(&(*s).monitor); 
    (*s).tr--; 
    if((*s).tr==0) 
    {
        if((*s).ceka[1-vr]>0) 
        {
            (*s).u_restoranu=1-vr; 
            pthread_cond_broadcast(&(*s).uvjet[1-vr]); 
        }

        else 
        {
            (*s).u_restoranu=-1;
        }
        (*s).zvr=0; 
    }
    ispis(vr,"iz restorana");
    pthread_mutex_unlock(&(*s).monitor);
}
void programer1(int vr)
{
    usleep(rand()%100000); 
    udzi(vr);
    usleep(rand()%100000); 
    izadzi(vr);
}
void programer2(int vr)
{
    usleep(rand()%100000); 
    udzi(vr);
    usleep(rand()%100000); 
    izadzi(vr);
}

void brisi(int sig)
{
    pthread_mutex_destroy(&(*s).monitor); 
    for(int i=0;i<2;i++)
        pthread_cond_destroy(&(*s).uvjet[i]);   
    shmdt(s);
    shmctl(id_podjeljeno,IPC_RMID,NULL);
}
int main(int argch, char **argv)
{
    sigset(SIGINT,brisi);
    int mjesta = atoi(argv[1]);
    int ljudi = atoi(argv[2]);
    
    
    //postavi
    postavi(mjesta,ljudi);
    typedef int key_t;
    id_podjeljeno=shmget(IPC_PRIVATE,sizeof(shared),0600);
    s=(shared*) shmat(id_podjeljeno,NULL,0);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(*s).monitor,&attr); 

    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    for(int i=0;i<2;i++)
        pthread_cond_init(&(*s).uvjet[i],&cattr); 
    //postavi end

    //starter
    int polje[ljudi*2];
    for(int i=0;i<ljudi*2;i++) 
        polje[i]=i%2; 
    for(int i=0;i<ljudi*2;i++)
        if(i%2)
        {
            switch (fork())
            {
                case 0:
                    {
                        programer2(polje[i]);
                        exit(0);
                    }
                case -1: 
                    cout<<"nije mogao biti stvoren proces"<<endl;break;
                default:
                    break;
            }
        }
        else
        {
            switch (fork())
            {
                case 0:
                    {
                        programer1(polje[i]);
                        exit(0);
                    }
                case -1: 
                    cout<<"nije mogao biti stvoren proces"<<endl;break;
                default:
                    break;
            }
        }
    for(int i=0;i<ljudi*2;i++)
        wait(NULL);
    //starter end
    brisi(1);
}