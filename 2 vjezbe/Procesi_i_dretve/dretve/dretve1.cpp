#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int globalna=10;

void *dretva(void *arg)
{
 globalna = globalna +1;
 return NULL;
}


int main()
{
 pthread_t tid;
 int retval;

 globalna = globalna + 10;

 retval=pthread_create(&tid,NULL,&dretva,NULL);
 if(retval<0)
 {
  printf("Pogreska\n");
  exit(1);
 }

 globalna = globalna +2;

 pthread_join(tid,NULL);

 printf("Ukupno = %d\n",globalna);

 return 0;
}
