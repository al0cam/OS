/****** dretva.c  *****/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define N 4

double a[100];
double suma[N];

void * dretva(void *arg);

int main()
{
 pthread_t tid[N];
 int i,retval, rdb[N];
 double ukupno=0.0;

 for(i=0;i<100;i++)
     a[i]=i+0.1;

 for(i=0;i<N;i++){
 rdb[i]=i;

 retval=pthread_create(&tid[i],NULL,&dretva,&rdb[i]);
 if(retval<0)
 {
  printf("Pogreska\n");
  exit(1);
 }
 }

 for(i=0;i<N;i++){
    pthread_join(tid[i],NULL);
    ukupno +=suma[i];
    }

 printf("Ukupno = %lf\n",ukupno);

 return 0;
}


void *dretva(void *arg)
{
  int i,j;
  j= *((int*)arg);

  suma[j]=0.0;
  for(i=j;i<100;i+=N)
     suma[j] += a[i];

 printf("Suma[%d]=%lf\n",j,suma[j]);
 return (void*)&suma[j];
}


