#include <cstdlib>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <cstring>

glavni program (parametri) 
{
   pokreni proces generiraj;
   pokreni m procesa racunaj;   
   čekaj na završetak svih procesa;
}

proces generiraj 
{
   dok (nije generirano n zadataka)
   {
      generiraj slučajni broj;
      spremi ga u dijeljenu varijablu;
      javi procesima da je stavljeni novi slučajni broj; //rabiti semafor
      čekaj da neki proces pročita slučajni broj; //rabiti semafor
   }
}

proces racunaj 
{
   dok (ima zadataka) 
   {
     čekaj da proces generiraj stavi sljedeći broj; //rabiti semafor
     pročitaj dijeljenu varijablu;
     javi procesu generiraj da je dijeljena varijabla pročitana; //rabiti semafor
     izračunaj zbroj;
     ispiši zbroj;
   }
}