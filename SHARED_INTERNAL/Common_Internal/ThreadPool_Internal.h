#pragma once
#ifndef CYBERGUARDIAN_THREADPOOL_INTERNAL_H
#define CYBERGUARDIAN_THREADPOOL_INTERNAL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <memory>

namespace CyberGuardian {
namespace Internal {
namespace Common {

class ThreadPool_Exs {
private:
    std::vector<std::thread> workers_Exs;
    std::queue<std::function<void()>> tasks_Exs;
    
    std::mutex queueMutex_Exs;
    std::condition_variable condition_Exs;
    std::atomic<bool> stop_Exs;
    std::atomic<size_t> activeTasks_Exs;
    
public:
    ThreadPool_Exs(size_t threads);
    ~ThreadPool_Exs();
    
    template<class F, class... Args>
    auto Enqueue_Exs(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    size_t GetQueueSize_Exs() const;
    size_t GetActiveTasks_Exs() const;
    size_t GetTotalThreads_Exs() const;
    
    void WaitAll_Exs();
    void Stop_Exs();
    
private:
    void WorkerThread_Exs();
};

template<class F, class... Args>
auto ThreadPool_Exs::Enqueue_Exs(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    
    using return_type = typename std::result_of<F(Args...)>::type;
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> result = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queueMutex_Exs);
        
        if (stop_Exs) {
            throw std::runtime_error("Enqueue on stopped ThreadPool");
        }
        
        tasks_Exs.emplace([task](){ (*task)(); });
    }
    
    condition_Exs.notify_one();
    return result;
}

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
