#ifndef INCLUDED_THREAD_QUEUE_HPP
#define INCLUDED_THREAD_QUEUE_HPP

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>


namespace thread_queue {

using task_t = std::move_only_function<int()>;

class thread_queue {
public:
    bool empty() const noexcept;

    void push(task_t&& task);

    void waitNPop(task_t& task);

    void done();

private:
    std::mutex qmut_; 
    std::condition_variable condCons_;
    std::queue<task_t> queue_;
};

}


#endif // INCLUDED_THREAD_QUEUE_HPP

