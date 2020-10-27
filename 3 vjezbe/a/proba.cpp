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
      printf("Nema semafora!\n");
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

void brisi(int sig)
{
    //std::cout<<"brisi"<<std::endl;
    SemRemove();
    exit(0);
}
int main()
{
   sigset(SIGINT,brisi);
    SemGet(1);
    std::cout<<"SemSetval: "<<SemSetVal(0,3)<<std::endl;
    std::cout<<"SemGetval: "<<SemGetVal(0)<<std::endl;
    std::cout<<"SemOp: "<<SemOp(0,1)<<std::endl;
    std::cout<<"SemOp: "<<SemOp(0,-1)<<" SemGetval: "<<SemGetVal(0)<<std::endl;
    std::cout<<"end"<<std::endl;
    SemRemove();
    return 0;
}