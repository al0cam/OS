#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

std::mutex mtx;
std::condition_variable cond;
int brojac=0;
void funkcija(int id)
{
    int broj;
    mtx.lock();  
    std::cout<<id<<": unesi broj"<<std::endl; 
    std::cin>>broj;
    std::cout<<id<<": otkljucavam mtx, brojac: "<<brojac<<std::endl; 
    mtx.unlock();
    std::unique_lock<std::mutex> lck(mtx);
    brojac++;
    cond.wait(lck);

    //mtx.lock();
    std::cout<<id<<": "<<broj<<std::endl;
    //mtx.unlock();
}
int main()
{
    std::vector<std::thread> l;
    for(int i=0;i<2;i++)
        l.push_back(std::thread(funkcija,i));
    while (brojac<2);
    cond.notify_all();
    
    for(auto& i:l)
        i.join();
    l.clear();
}

