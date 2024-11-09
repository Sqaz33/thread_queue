#include "../include/thread_queue.hpp"

#include <future>
#include <utility>
#include <tuple>
#include <type_traits>

namespace thread_queue {

template <> 
task_t getLimiter<task_t>() {
    auto t = [] () { return static_cast<int>(Limiter::STOP); };
    thread_queue::task_t stop(t);
    return std::move(stop);
}

bool UnboundedQueue::empty() const noexcept {
    std::lock_guard<std::mutex> lk{qmut_};
    return queue_.empty();
}

void UnboundedQueue::push(task_t&& task) {
    std::lock_guard<std::mutex> lk{qmut_};
    queue_.push(std::move(task));
    condCons_.notify_one();
}

void UnboundedQueue::waitNPop(task_t& task) {
    std::unique_lock<std::mutex> lk{qmut_};
    condCons_.wait(lk, [this]{ return !queue_.empty(); });
    decltype(auto) frnt = std::move(queue_.front());
    task = std::move(frnt);
}

void UnboundedQueue::wakeUpAllConsumer() {
    queue_.push(getLimiter<task_t>());
    condCons_.notify_all();
}

} // namespace thread_queue