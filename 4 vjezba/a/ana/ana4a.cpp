#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

pthread_mutex_t m;
pthread_cond_t r;

int stanje;
int N;

void pocetak(int broj_dretvi){
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&r, NULL);
	N=broj_dretvi;
	stanje=0;
}
void zakljucaj(){
	pthread_mutex_lock(&m);
}
void otkljucaj(){
	pthread_mutex_unlock(&m);
}
void cekaj(){
	pthread_cond_wait(&r, &m);
}
void javi(){
	pthread_cond_broadcast(&r);
}
void *barijera(void* vrijednost){
    int id_dretve = *((int *)vrijednost);
	int unos=0;
	zakljucaj();
	stanje++;
	if(stanje<N){
        printf("Dretva %d. unesite broj\n ",id_dretve );
		cin >> unos;
		cekaj();
	} 
	else{
        printf("Dretva %d. unesite broj\n ",id_dretve );
		stanje = 0;
		cin >> unos;
		printf("\n");
		javi();
	}
	otkljucaj();
	zakljucaj();
	stanje++;
	if(stanje < N){
		printf("Dretva %d broj: %d\n",id_dretve,unos);
		cekaj();
	} else{
		stanje = 0;
		printf("Dretva %d broj: %d\n",id_dretve,unos);
		javi();
	}
	otkljucaj();
}

void brisi(int signal){
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&r);
	exit(0);
}

int main(int arg_count, char** argv){
	sigset(SIGINT, brisi);
	if(arg_count != 2 || atoi(argv[1]) < 1) {
		printf("Neispravno!\n");
		exit(1);
    }
	int broj_dretvi=atoi(argv[1]);
	printf("Broj dretava: %d\n",broj_dretvi);
	pocetak(broj_dretvi);
	pthread_t dretve[broj_dretvi];
	int broj_dretve[broj_dretvi];
	
    for(int i = 0; i < broj_dretvi; i++) {
		broj_dretve[i] = i;
		if(pthread_create(&dretve[i], NULL, barijera, &broj_dretve[i]) != 0) {
			printf("Greska pri stvaranju prve dretve!\n");
			exit(1);
		}
	}
	for(int j=0; j<N; j++)
		pthread_join(dretve[j], NULL);
	brisi(0);
}
