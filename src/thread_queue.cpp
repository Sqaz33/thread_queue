#include "../include/thread_queue.hpp"

#include <future>
#include <utility>
#include <tuple>
#include <type_traits>

namespace thread_queue {

bool UnboundedQueue::empty() const noexcept {
    std::lock_guard<std::mutex> lk{qmut_};
    return queue_.empty();
}

void UnboundedQueue::pushTask(task_t&& task) {
    std::lock_guard<std::mutex> lk{qmut_};
    queue_.push(std::move(task));
    condCons_.notify_one();
}

void UnboundedQueue::waitNPop(task_t& task) {
    std::unique_lock<std::mutex> lk{qmut_};
    condCons_.wait(lk, [this]{ return !empty(); });
    decltype(auto) frnt = std::move(queue_.front());
    //TODO:
    // if (what) { return; }
    task = std::move(frnt);
}

void UnboundedQueue::done() {
    //TODO: что использовать в качистве Limiter?
    queue_.push(GetLimiter<task_t>());
    condCons_.notify_all();
}

} // namespace thread_queue