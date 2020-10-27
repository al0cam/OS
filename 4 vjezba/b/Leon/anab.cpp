#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <sys/shm.h>
#include <cstring>
using namespace std;
struct dijeljeno{
    int tr, zajednicka, ceka[2]; 
    int u_restoranu=-1; 
    pthread_mutex_t monitor;
    pthread_cond_t uvjet[2]; 
} *d;

int id_podjeljeno;
pthread_mutexattr_t attr;
pthread_condattr_t cattr;

void pocetak(){
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(*d).monitor,&attr); 

    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    for(int i=0;i<2;i++)
        pthread_cond_init(&(*d).uvjet[i],&cattr); 

}

void zakljucaj(){
	pthread_mutex_lock(&(*d).monitor);
}
void otkljucaj(){
	pthread_mutex_unlock(&(*d).monitor);
}
void cekaj(int vrijednost){
	pthread_cond_wait(&(*d).uvjet[vrijednost], &(*d).monitor);
}
void javi(int vrijednost){
	pthread_cond_broadcast(&(*d).uvjet[1-vrijednost]);
}

void u_redu(char slovo, int ukupno)
{
    
    
}
void restoran(int vrijednost,string tx)
{
    printf("Restoran:");
    if((*d).u_restoranu==1){
        for(int i=0;i<(*d).tr;i++)
            printf("M");
    }
    else{
        for(int i=0;i<(*d).tr;i++)
            printf("L");
    }
    printf("--->");
    if(vrijednost==1){
        printf("M");
    }
    else{
        printf("L");
    }
    printf("\n");
}
void ispis(int vrijednost, string tx, int ukupno)
{
    printf("Red Linuxasa:"); 
    for(int i=0;i<ukupno;i++)
        if(i<(*d).ceka[0]) 		
            printf("L"); 
        else
            printf("-");
    printf("Red Microsofva:");
    for(int i=0;i<ukupno;i++)
        if(i<(*d).ceka[0]){
            printf("M");
        }	
        else{
            printf("-");
        }
    restoran(vrijednost,tx);
}
void udzi(int &vrijednost, int dozvoljeno,int ukupno) 
{
    zakljucaj(); 
    (*d).ceka[vrijednost]++;  
    while((*d).zajednicka>=dozvoljeno||(*d).u_restoranu==1-vrijednost){
        ispis(vrijednost, "u redu cekanja", ukupno);
        cekaj(vrijednost); 
    }           

    (*d).tr++; 
    (*d).u_restoranu=vrijednost; 
    (*d).ceka[vrijednost]--; 
    if((*d).ceka[1-vrijednost]>0){
        (*d).zajednicka++; 
    } 
    ispis(vrijednost," u restoranu",ukupno); 
    otkljucaj();
}
void izadzi(int &vrijednost, int ukupno) 
{
    zakljucaj(); 
    (*d).tr--; 
    if((*d).tr==0){
        if((*d).ceka[1-vrijednost]>0){
            (*d).u_restoranu=1-vrijednost; 
            javi(vrijednost); 
        }
        else{
            (*d).u_restoranu=-1;
        }
        (*d).zajednicka=0; 
    }
    ispis(vrijednost,"iz restorana",ukupno);
    otkljucaj();
}

void brisi(int sig)
{
    pthread_mutex_destroy(&(*d).monitor); 
    for(int i=0;i<2;i++)
        pthread_cond_destroy(&(*d).uvjet[i]);   
    shmdt(d);
    shmctl(id_podjeljeno,IPC_RMID,NULL);
}
int main(int argch, char **argv)
{
    sigset(SIGINT,brisi);
    int dozvoljeno = atoi(argv[1]);
    int ukupno = atoi(argv[2]);
    
    typedef int key_t;
    id_podjeljeno=shmget(IPC_PRIVATE,sizeof(dijeljeno),0600);
    d=(dijeljeno*) shmat(id_podjeljeno,NULL,0);
    pocetak();
    int polje[ukupno*2];
    for(int i=0;i<ukupno*2;i++) 
        polje[i]=i%2;

    srand(time(0));
    rand();
    for(int i=0;i<ukupno*2;i++)
        if(i%2)
        {
            switch (fork())
            {
                case 0:
                    {
                        
                        usleep(rand()%10000); 
                        udzi(polje[i],dozvoljeno,ukupno);
                        usleep(rand()%10000); 
                        izadzi(polje[i],ukupno);
                        exit(0);
                    }
                case -1: 
                    printf("Greska pri stvaranju procesa\n");
                    exit(0);
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
                        usleep(rand()%10000); 
                        udzi(polje[i],dozvoljeno,ukupno);
                        usleep(rand()%10000); 
                        izadzi(polje[i],ukupno);
                        exit(0);
                    }
                case -1: 
                    printf("Greska pri stvaranju procesa\n");
                    exit(0);
                default:
                    break;
            }
        }
    for(int i=0;i<ukupno*2;i++)
        wait(NULL);
    brisi(0);
}