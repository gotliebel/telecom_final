#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;
int var = 0;

void writerThread() {
  for (int i = 0; i < 1000; ++i) {
    std::lock_guard<std::mutex> lock(m);
    var++;
  }
}

void readerThread() {
  for (int i = 0; i < 1000; ++i) {
    std::lock_guard<std::mutex> lock(m);
    std::cout << "Shared variable: " << var << '\n';
  }
}

int main() {
  std::thread t1(writerThread);
  std::thread t2(readerThread);

  t1.join();
  t2.join();

  return 0;
}
