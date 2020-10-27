#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
std::mutex mtx;
std::condition_variable cond;
int brojac=0;
void funkcija(int br)
{
    int broj;
    mtx.lock();
    std::cout<<std::this_thread::get_id()<<std::endl;
    std::cout<<"do it work"<<std::endl;
    std::cin>>broj;
    std::cout<<brojac<<std::endl;
    mtx.unlock();
    std::cout<<"prije uvjeta"<<std::endl;
    std::unique_lock<std::mutex> lck(mtx);
    if(++brojac<br) cond.wait(lck);
    else 
    {
        cond.notify_all();
    }
    std::cout<<broj<<std::endl;
}
int main()
{   
    std::vector<std::thread> t;
    int broj;
    std::cout<<"koliko threadova: "; std::cin>>broj;
    for(auto i=0;i<broj;i++)
        t.push_back(std::thread(funkcija,broj));
    for(auto& i:t) i.join();
    t.clear();
    
}