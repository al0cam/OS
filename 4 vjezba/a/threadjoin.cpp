#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <vector>
 
void pause_thread(int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}
 
int main() 
{
  std::cout << "Spawning 3 threads...\n";
  std::vector<std::thread> t;
  t.push_back(std::thread(pause_thread,1));
  t.push_back(std::thread(pause_thread,2));
  t.push_back(std::thread(pause_thread,3));
  std::cout << "Done spawning threads. Now waiting for them to join:\n";
  for(auto& i:t)
    i.join();
    t.clear();
  std::cout << "All threads joined!\n";

  return 0;
}