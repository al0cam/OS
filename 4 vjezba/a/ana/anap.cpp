#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int PROSTOR, n, *TRAZIM, *BROJ;

int max() {
	int a;
	int max = BROJ[0];
	for(a = 0; a < n; a++)
		if(max < BROJ[a])
			max = BROJ[a];
	return max;
}

void ulaz(int i) {
	int j;
	TRAZIM[i] = 1;
	BROJ[i] = max() + 1;
	TRAZIM[i] = 0;
	
	for(j = 0; j < n; j++)	{
		while(TRAZIM[j] != 0);
		while(BROJ[j] != 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)));
	}
}

void izlaz(int i) {
	BROJ[i] = 0;
}

void proces(int i) {
	int k, m;
	for(k = 0; k < 5; k++) {
		ulaz(i);
		for(m = 0; m < 5; m++) {
			printf("Proces: %d K.O. br: %d (%d/5)\n", i+1, k+1, m+1);
			sleep(1);
		}
		izlaz(i);
	}
}

void brisi(int sig) {
	shmdt(BROJ);
	shmdt(TRAZIM);
	shmctl(PROSTOR, IPC_RMID, NULL);
	exit(0);
}

int main(int arg_count, char *argv[]){
	if(arg_count != 2 || atoi(argv[1]) < 1) {
		cout << "Neispravno uneseni parametri!" << endl;
		exit(1);
	}
	
	n = atoi(argv[1]);
	sigset(SIGINT, brisi);
	
	PROSTOR = shmget(IPC_PRIVATE, sizeof(int), 0600);
	
	if(PROSTOR == -1)
		exit(1);
	
    TRAZIM = (int *)shmat(PROSTOR, NULL, 0);
    BROJ = (int *)shmat(PROSTOR, NULL, 0) + sizeof(int);
	
	int i;
	for(i = 0; i < n; i++) {
		switch(fork()) {
			case 0:
				proces(i);
				exit(0);
			case -1:
				cout << "Greska pri stvaranju procesa!" << endl;
				exit(0);
			default:
				break;
		}
	}
	
	for(i = 0; i < n; i++)
		wait(NULL);
	
	brisi(0);
	
	return 0;
}
