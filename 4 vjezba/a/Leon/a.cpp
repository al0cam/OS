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

pthread_mutex_t monitor;
pthread_cond_t red;

int stanje_Reda;
int N;

void *Barijera(void* argument){
	int *Id=(int*) argument;
	int vrijednost=0;
	pthread_mutex_lock(&monitor);
	stanje_Reda++;
	if(stanje_Reda<N){
		cout << "Dretva " << *Id << ". unesite broj\n";
		cin >> vrijednost;
		pthread_cond_wait(&red, &monitor);
	} 
	else{
		cout << "Dretva " << *Id << ". unesite broj\n";
		cin >> vrijednost;
		cout << endl;
		stanje_Reda = 0;
		pthread_cond_broadcast(&red);
	}
	pthread_mutex_unlock(&monitor);
	pthread_mutex_lock(&monitor);
	stanje_Reda++;
	if(stanje_Reda < N){
		cout << "Dretva " << *Id << ". uneseni broj je: " << vrijednost << endl;
		pthread_cond_wait(&red, &monitor);
	} else{
		cout << "Dretva " << *Id << ". uneseni broj je: " << vrijednost << endl;
		stanje_Reda = 0;
		pthread_cond_broadcast(&red);
	}
	pthread_mutex_unlock(&monitor);
}

void brisi(int signal){
	pthread_mutex_destroy(&monitor);
	pthread_cond_destroy(&red);
	exit(0);
}

int main(int argc, char** argv){
	sigset(SIGINT, brisi);
	if(argc!=2){
		cout << "Potrebno je unijeti jedan argument!\n";
		exit(1);
	}
	int broj_dretvi=atoi(argv[1]);
	cout << "Broj dretvi=" << broj_dretvi << endl;

    pthread_cond_init(&red, NULL);
	pthread_mutex_init(&monitor, NULL);
    
    N=broj_dretvi;
	stanje_Reda=0;
	
    
	pthread_t tid[broj_dretvi];
	int m[broj_dretvi];
	for(int i=0; i<N; i++){
		m[i]=i;
		if (pthread_create(&tid[i], NULL, Barijera, &m[i])!=0){
			cout << "Pogreska!\n" ;
			exit(1);
		}
	}
	for(int i=0; i<N; i++)
		pthread_join(tid[i], NULL);
	brisi(0);
}
