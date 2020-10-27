#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <cstring>

int SemId;   /* identifikacijski broj skupa semafora */

void SemGet(int n)
{  /* dobavi skup semafora sa ukupno n semafora */
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      std::cout<<"Nema semafora!\n";
      exit(1);
   }
}

int SemSetVal(int SemNum, int SemVal)
{  /* postavi vrijednost semafora SemNum na SemVal */
   return semctl(SemId, SemNum, SETVAL, SemVal);
}

int SemGetVal(int SemNum)
{  /* citaj vrijednost semafora SemNum na SemVal */
   return semctl(SemId, SemNum, GETVAL, NULL);
}

int SemOp(int SemNum, int SemOp)
{  /* obavi operaciju SemOp sa semaforom SemNum */
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, &SemBuf, 1);
}

void SemRemove()
{  /* uništi skup semafora */
   semctl(SemId, 0, IPC_RMID, 0);
}

class semafor
{
    private:
        int broj, *podjeljeno, *procesi, id_procesa, id_podjeljeno, br_procesa, broj_semafora;
    public:
        int postavi(int broj_procesa, int broj_gl)
        {

            br_procesa=broj_procesa+1;
            broj_semafora=broj_procesa;
            broj=broj_gl;
            
            typedef int key_t;
            id_podjeljeno=shmget(IPC_PRIVATE,sizeof(int),0600);
            podjeljeno= (int*) shmat(id_podjeljeno,NULL,0);
            *podjeljeno=0;

            id_procesa=shmget(IPC_PRIVATE,sizeof(int),0600);
            procesi= (int*) shmat(id_procesa,NULL,0);
            *procesi=0;
            SemGet(broj_semafora);
            for(int i=0;i<broj_semafora;i++)
                SemSetVal(i,0);
        }
        int generiraj()
        {
            std::cout<<"PROCES"<<" generiranje STVOREN"<<std::endl;
            srand(time(0));
            rand();
            int generirano,brojac=0;
            while(brojac++<broj)
            {
                generirano=rand();
                *podjeljeno=generirano;
                std::cout<<"Generiran broj: "<<*podjeljeno<<std::endl;
                for(int i=0;i<broj_semafora;i++)
                    if(SemGetVal(i)) ;
                    else
                    {
                        SemOp(i,1);
                        break;
                    } 
                bool freespace;
                do
                {   
                    freespace=true;
                    for(int i=0;i<broj_semafora;i++)
                        if(SemGetVal(i)) freespace=false;
                    /*std::cout<<"SemGetVal(0): "<<SemGetVal(0)<<std::endl;*/
                }while(!freespace);
            }
            *podjeljeno=-1;
        }
        void racunaj()
        {
            int broj_procesa=*procesi-1;
            int broj_semafora_u_procesu=broj_procesa-1;
            std::cout<<"PROCES "<<broj_procesa<<". STVOREN"<<std::endl;
            sleep(1);
            while(*podjeljeno>=0)
            {
                if(*podjeljeno<0) break;
                while(!SemGetVal(broj_semafora_u_procesu))
                {
                    if(*podjeljeno<0) break;
                }
                std::cout<<"PROCES "<<broj_procesa<<". preuzeo zadatak: "<<*podjeljeno<<std::endl;
                if(*podjeljeno<0) break;
                int posebna_varijabla=*podjeljeno;
                SemOp(broj_semafora_u_procesu,-1);
                int zbroj=0;
                for (int i = 0; i <= posebna_varijabla; i++)
                    zbroj+=i;
                std::cout<<"PROCES "<<broj_procesa<<". zbroj: "<<zbroj<<std::endl;
            }
        }
        void starter()
        {
            int i=0;
            for(i=0;i<br_procesa;i++)
                {
                    switch (fork())
                    {
                        case 0:
                            {
                                if((*procesi)==0)
                                {
                                    (*procesi)++;
                                    generiraj();
                                }
                                else
                                {
                                    //std::cout<<"PROCES "<<*procesi<<". STVOREN"<<std::endl;
                                    (*procesi)++;
                                    racunaj();
                                }
                                exit(0);
                            }
                        case -1: 
                            std::cout<<"nije mogao biti stvoren proces"<<std::endl;break;
                        default:
                            break;
                    }
                }
            for(i=0;i<*procesi;i++)
                wait(NULL);
            
        }

        ~semafor()
        {
            std::cout<<"Kill"<<std::endl;
            for(int i=0;i<br_procesa;i++) wait(NULL);
            SemRemove();
            shmdt(podjeljeno);
            shmctl(id_podjeljeno,IPC_RMID,NULL);
            shmdt(procesi);
            shmctl(id_procesa,IPC_RMID,NULL);
        }
};
semafor *s=new semafor;
void brisi(int sig)
{
    delete s;
    exit(0);
}
int main(int argch,char **argv)
{
    sigset(SIGINT,brisi);
    int broj_procesa=atoi(argv[1]);
    int broj_elemenata=atoi(argv[2]); 
    (*s).postavi(broj_procesa,broj_elemenata);
    (*s).starter();
    delete s;
    return 0;
}

