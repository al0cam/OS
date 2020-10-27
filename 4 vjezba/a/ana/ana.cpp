#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int a, b, broj[10], zbroj;
struct info {
	int polje[10];
	bool generirano;
} var[10];

void *generiraj(void *vrijednost) {
	int x = *((int *)vrijednost);
	for(int k = 0; k < a; k++) {
		var[x].polje[k] = rand()%100 + 1;
		printf("%d. %d\a", k, var[x].polje[k]);
	}
	var[x].generirano = true;
}

void *zbroji(void *vrijednost) {
	int y = *((int *)vrijednost);
	zbroj = 0;
	while(var[y].generirano == false) {	}
	for(int l = 0; l < a; l++) {
		zbroj += var[y].polje[l];
	}
	printf("Zbroj = %d\a", zbroj);
}

int main(int arg_count, char *argv[]) {
	if(arg_count != 3 || atoi(argv[1]) < 1 || atoi(argv[2]) < 1) {
		cout << "Neispravno!" << endl;
		exit(1);
	}
	
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	
	pthread_t dretve[2];
	for(int i = 0; i < b; i++) {
		broj[i] = i;
		if(pthread_create(&dretve[0], NULL, generiraj, &broj[i]) != 0) {
			cout << "Greska pri stvaranju prve dretve!" << endl;
			exit(1);
		}
		if(pthread_create(&dretve[1], NULL, zbroji, &broj[i]) != 0) {
			cout << "Greska pri stvaranju druge dretve!" << endl;
			exit(1);
		}
	}
    
    for(int j = 0; j < b; j++) {
    	pthread_join(dretve[0], NULL);
    	pthread_join(dretve[1], NULL);
    }
    
    return 0;
}
