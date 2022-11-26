#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals; //1ms

template <class T>
struct Mutex {
  Mutex() {}

  Mutex(T v): value(v) {}

  ~Mutex() {
    mutex.unlock();
  }

  auto lock() -> void {
    mutex.lock();
  }

  auto unlock() -> void {
    mutex.unlock();
  }

  auto update(T t) -> void {
    value = t;
  }

  auto update(T(*fn)(T)) -> void {
    value = fn(value);
  }

  auto get() -> T {
    return value;
  }

private:
  T value;
  std::mutex mutex;
};

auto main(void) -> int {
  Mutex<int> m = Mutex<int>(0);
  
  auto fn = [&](auto id) mutable {
    for(auto i = 0; i < 10; ++i) {
      {
        std::lock_guard<Mutex<int>> lock(m);
        m.update([](auto t) {
          return t + 1;
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
