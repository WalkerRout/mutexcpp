#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>
#include <thread>

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

  template <class F>
  auto update(F fn) -> void {
    value = fn(value);
  }

  auto get() -> T {
    return value;
  }

private:
  T value;
  std::mutex mutex;
};

#endif // MUTEX_H