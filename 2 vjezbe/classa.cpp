#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include <sys/shm.h>
#include <ctime>
#include <cstring>
using namespace std;

class livada{
    private:
        //basic stuff, inicijalizacija za kasnije
        int *polje_za_brojeve,
            broj_podjela,
            *polje_za_snalazenje,
            *brojac,
            id;
    public:
        livada(int broj_elemenata)
        {
            //stvaranje polja u koje cemo generirati brojeve
            polje_za_brojeve=new int[broj_elemenata];
        }
        void gen(int broj_elemenata)
        {
            //generator brojeva
            srand(0);
            for(int i=0;i<broj_elemenata;i++) 
                polje_za_brojeve[i]=rand()%9;
            
        }
        void ispis(int broj_elemenata)
        {
            for (int i=0;i<broj_elemenata;i++)
                cout<<i<<": "<<polje_za_brojeve[i]<<endl;
        }
        void dijeli(int broj_elemenata, int velicina_polja)
        {
            double broj_nezaokruzen;
            int velicina_segmenta=broj_elemenata;
            //racunanje broja segmenata
            broj_nezaokruzen=(double)broj_elemenata/velicina_polja;
            broj_podjela=(int)broj_nezaokruzen;
            broj_nezaokruzen-=broj_podjela;
            if(broj_nezaokruzen>0) broj_podjela++;
            //stvaranje novog polja velicine jednake broju segmenata
            polje_za_snalazenje=new int[broj_podjela];

            //izracun koliko elemenata ima u svakom semgentu, tj postavljanje u polje koliko je koji segment velik
            for(int i=0;i<broj_podjela;i++)
            {
                if(velicina_segmenta>velicina_polja) polje_za_snalazenje[i]=velicina_polja;
                else polje_za_snalazenje[i]=velicina_segmenta;
                velicina_segmenta-=velicina_polja;
            }
        }
        void procesi(int velicina_polja)
        {
            //inicijalizacija, pokazivac brojac koji ce pokazivati na zajednicku memoriju
            typedef int key_t; 
            int i;
            //zajednicka memorija samo za brojac, jer mi vise od toga nije potrebno
            id=shmget(IPC_PRIVATE,sizeof(int),0600);
            if(id==-1) exit(1);
            //postavljanje pokazivaca na zajednicku memoriju
            brojac=(int*) shmat(id,NULL,0);
            *brojac=0;
            cout<<"Broj procesa: "<<broj_podjela<<endl;
            //stvaranje odredenog broja procesa koji obavljaju neko zbrajanje
            for(i=0;i<broj_podjela;i++)
                switch (fork())
                {
                    case 0:
                        {
                            int j=0,suma=0,brojac_za_ispis=*brojac;
                            while(j++<polje_za_snalazenje[i])
                                suma+=polje_za_brojeve[(*brojac)++];
                            cout<<"Suma("<<brojac_za_ispis<<"-"<<*brojac<<"): "<<suma<<endl;
                            exit(0);
                        }
                    case -1: 
                        cout<<"nije mogao biti stvoren proces"<<endl;break;
                    default:
                        break;
                }
            //cekanje da procesi zavrse
            while(i--)wait(NULL);
            //oslobadjanje zajednicke memorije
            shmdt(brojac);
            shmctl(id,IPC_RMID,NULL);

        }
        ~livada()
        {
            //oslobadjanje memorije
            shmdt(brojac);
            shmctl(id,IPC_RMID,NULL);
            delete []polje_za_brojeve;
            delete []polje_za_snalazenje;
        }

};
void brisi(int sig)
{
    exit(0);
}

int main(int argch, char **argv)
{
    sigset(SIGINT,brisi);
    int broj_elemenata=stoi(argv[1]);
    int velicina_polja=stoi(argv[2]);

    if(broj_elemenata>10000) broj_elemenata=10000;

    livada l(broj_elemenata);

    l.gen(broj_elemenata);
    l.ispis(broj_elemenata);
    l.dijeli(broj_elemenata,velicina_polja);
    l.procesi(velicina_polja);
    
    return 0;
}