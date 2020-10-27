#include <iostream>
#include <cstdlib>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void proces()
{

}

int main()
{
    for(int i=0;i<2;i++)
    {
        switch (fork())
        {
        case 0: proces(); break;
        case -1: exit(0); break;
        default:
            break;
        }
    }
    for(int i=0;i<2;i++)
        wait(NULL);
}