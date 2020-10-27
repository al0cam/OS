#include <iostream>
#include <cstdlib>
#include <thread>

using namespace std;
class mamu_ti_jebem
{
    private:
        int broj;
        
    public:
        void funkcija()
        {
            cin>>broj;
            cout<<broj<<endl;
        }
        
        ~mamu_ti_jebem()
        {
            
        }
};
void druga_funkcija()
{
    
    mamu_ti_jebem u_picku;
    u_picku.funkcija();
    cout<<"done"<<endl;
}

int main()
{
    mamu_ti_jebem u_picku;   
    //void (*fun)()=mamu_ti_jebem::funkcija();
    void (mamu_ti_jebem::*fp1)()=&mamu_ti_jebem::funkcija;
    //fun();
    (u_picku.*fp1)();
    cout<<"amo dalje"<<endl;
    for(int i=0;i<3;i++)
    thread t[i](druga_funkcija);
    for(int i=0;i<3;i++)
    t[i].join();
    return 0;
}


/*class Data
{ 
    public:
    int f(float) 
    { 
        return 1; 
    }
};

int (Data::*fp1) (float) = &Data::f;   // Declaration and assignment
int (Data::*fp2) (float);        // Only Declaration

int main(0
{
    fp2 = &Data::f;   // Assignment inside main()
}*/