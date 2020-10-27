#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct shared
std::mutex mtx;
std::unique_lock<std::mutex> lck(mtx);

std::condition_variable cond;

void funkcija(int broj)
{
    
    std::cout<<"get cancer"<<std::endl;
    lck.lock();
    std::cout<<"."<<std::endl;
    std::cout<<broj<<std::endl;
    lck.unlock();
    /*if(broj!=1)cond.wait(lck);
    else 
    {
        cond.notify_all();
    }*/
}

int main()
{
    for(int i=0;i<2;i++)
        switch (fork())
        {
            case 0:
                {
                    funkcija(i);
                    exit(0);
                }
            case -1: 
                std::cout<<"nije mogao biti stvoren proces"<<std::endl;break;
            default:
                break;
        }
        
    for(int i=0; i<2; i++)
        wait(NULL);
}