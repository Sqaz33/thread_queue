#include <iostream>
#include <exception>
#include <thread>

#include "../include/thread_queue.hpp"

thread_queue::UnboundedQueue q;

int work(int a, int b) {
    return a + b;
}

void consumerWork() {
    using namespace thread_queue;
    task_t t;
    q.waitNPop(t);
    int stop = std::move(t)();
    if (stop) q.wakeUpAllConsumer();
}

void produserWork() {
    using namespace thread_queue;
    auto [tsk, fut] = createTask(work, 1, 2);
    q.push(std::move(tsk));
    std::cout << fut.get() << '\n';
}

int main() try {
    std::vector<std::thread> vt;
    
    std::thread consumer(consumerWork);
    std::thread produser(produserWork);

    consumer.join();
    produser.join();
} 
catch(const std::exception& e) { std::cout << e.what() << '\n'; }
catch(...) { std::cout << "Unknow error.\n"; }