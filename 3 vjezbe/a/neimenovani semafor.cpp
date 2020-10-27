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
#include <semaphore.h>

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
int n=3;
struct dijeljeno
{
   int broj[3];
   char slovo;
};

int main()
{
    int id;
    dijeljeno *d;
    id=shmget(IPC_PRIVATE,sizeof(dijeljeno),0600);
    d=(dijeljeno*) shmat(id,NULL,0);
    std::cout<<"radi"<<std::endl;
    std::cout<<"stvoreno"<<std::endl;
    int *polje;
    polje=new int[10];
    std::cout<<"stvoreno"<<std::endl;

    polje[1]=2;
    std::cout<<"funky stuff"<<std::endl;
    std::cout<<polje[1]<<std::endl;
    std::cout<<"funky stuff"<<std::endl;
    std::cout<<"broj: "<<d->broj[0]<<std::endl;
    std::cout<<"radi"<<std::endl;


   sem_t *semafor=new sem_t;
   sem_init(semafor,1,0);
   sem_post(semafor);
   int vrijednost;
   sem_getvalue (semafor,&vrijednost);
   sem_wait(semafor);
   sem_destroy(semafor);
   
   std::cout<<"Vrijednost semafora: "<<vrijednost<<std::endl;
    shmdt(d);
    shmctl(id,IPC_RMID,NULL);
    return 0;
}