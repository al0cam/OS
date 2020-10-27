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

struct shared
{
    std::mutex monitor;
    std::condition_variable cond[2]; 
    int tr, zvr, ceka[2]; 
    int u_restoranu=-1; 
};
class programeri
{
    private:
        int id_podjeljeno; 
        int mjesta,ljudi;
        shared *s;
    public:
    void postavi(int mjesta_main, int ljudi_main)
    {
        mjesta=mjesta_main;
        ljudi=ljudi_main;
        typedef int key_t;
        id_podjeljeno=shmget(IPC_PRIVATE,sizeof(shared),0600);
        s=(shared*) shmat(id_podjeljeno,NULL,0);
        std::unique_lock<std::mutex> lck(s->monitor,std::defer_lock);
        
    }
    std::unique_lock<std::mutex> lck(s->monitor,std::defer_lock);
    void ispis(int vr, std::string tx)
    {
        std::cout<<"Red Linuxasa:"; 
        for(int i=0;i<ljudi;i++)
            if(i<s->ceka[0]) 		
                std::cout<<"L"; 
            else
                std::cout<<"-"; 

        std::cout<<"Red Microsoftasa:"; 
        for(int i=0;i<ljudi;i++)
            if(i<s->ceka[1])
                std::cout<<"M";
            else
                std::cout<<"-";

        std::cout<<"Restoran:";

        if(s->u_restoranu==1) 
            for(int i=0;i<s->tr;i++)
                std::cout<<"M";
        else 
            for(int i=0;i<s->tr;i++)
                std::cout<<"L";
        std::cout<<"-->";

        if(vr==1)
            std::cout<<"M "<<tx;
        else
            std::cout<<"L "<<tx;
        std::cout<<std::endl;
    }

    void udzi(int &vr) 
    {
        s->monitor.lock(); 
        s->ceka[vr]++;  

        while(s->u_restoranu==1-vr || s->zvr>=mjesta) 
        {
            ispis(vr, "u redu cekanja"); 
            s->cond[vr].wait(lck);
        }           

        s->ceka[vr]--; 
        s->u_restoranu=vr; 
        s->tr++; 
        if(s->ceka[1-vr]>0) 
            s->zvr++;  
        ispis(vr," u restoranu"); 
        s->monitor.unlock();
    }
    void izadzi(int &vr) 
    {
        s->monitor.lock();
        s->tr--; 
        if(s->tr==0) 
        {
            if(s->ceka[1-vr]>0) 
            {
                s->u_restoranu=1-vr; 
                pthread_cond_broadcast(&s->uvjet[1-vr]); 
            }

            else 
            {
                s->u_restoranu=-1;
            }
            s->zvr=0; 
        }
        ispis(vr,"iz restorana");
        s->monitor.unlock();
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
    void starter()
    {
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
                        std::cout<<"nije mogao biti stvoren proces"<<std::endl;break;
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
                        std::cout<<"nije mogao biti stvoren proces"<<std::endl;break;
                    default:
                        break;
                }
            }
        for(int i=0;i<ljudi*2;i++)
            wait(NULL);
    }

    ~programeri()
    {
        pthread_mutex_destroy(&s->monitor); 
        for(int i=0;i<2;i++)
            pthread_cond_destroy(&s->uvjet[i]);   
        shmdt(s);
        shmctl(id_podjeljeno,IPC_RMID,NULL);
    }
};

programeri *p=new programeri;
void brisi(int sig)
{
    delete p;
    exit(0);
}
int main(int argch, char **argv)
{
    sigset(SIGINT,brisi);
    int mjesta=atoi(argv[1]);
    int ljudi=atoi(argv[2]);
    p->postavi(mjesta,ljudi);
    p->starter();
    delete p;
}