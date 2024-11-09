#ifndef INCLUDED_THREAD_QUEUE_HPP
#define INCLUDED_THREAD_QUEUE_HPP

#include <condition_variable>
#include <future>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace thread_queue {

using task_t = std::move_only_function<int()>;

class UnboundedQueue {
public:
    bool empty() const noexcept;

    void push(task_t&& task);

    void waitNPop(task_t& task);

    void wakeUpAllConsumer();

private:
    mutable std::mutex qmut_; 
    std::condition_variable condCons_;
    std::queue<task_t> queue_;
};

template <typename F, typename... Args>
auto createTask(F f, Args&&... args) {
    std::packaged_task<std::remove_pointer_t<F>> ptsk{f};
    auto fut = ptsk.get_future();
    task_t tsk (
        [ct = std::move(ptsk), 
        args = std::make_tuple(std::forward<Args>(args)...)]() 
        mutable {
            std::apply(
                [ct = std::move(ct)] (auto&&... args) mutable {
                    ct(args...);
                },
                std::move(args)
            );
            return 0;
        }
    );
    return std::make_pair( std::move(tsk), std::move(fut) );
}

enum class Limiter : int { STOP = -1 };

template <typename T> T getLimiter();

} // namespace thread_queue

#endif // INCLUDED_THREAD_QUEUE_HPP

