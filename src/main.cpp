
#include <iostream>
#include <mutex>
#include <thread>

#include "mutex.h"

using namespace std::chrono_literals; //1ms

auto main(void) -> int {
  auto m = Mutex<int>(0);
  
  auto fn = [&](int id) mutable {
    for(auto i = 0; i < 10; ++i) {
      {
        std::lock_guard<Mutex<int>> lock(m);
        m.update([&](auto t) {
          return t + 1 + (id % 2 == 0);
        });
        std::cout << "From thread #" << id << ": " << m.get() << "\n";
        // lock_guard drops, mutex unlocks
      }
      std::this_thread::sleep_for(1ms);
    }
  };
  
  std::thread t1(fn, 1);
  std::thread t2(fn, 2);
  
  t1.join();
  t2.join();
  
  return 0;
}
