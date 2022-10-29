
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <condition_variable>
#include <mutex>

#include <functional>
#include <vector>
#include <queue>

#include <iostream>

class ThreadPool {

public:
    ThreadPool() : m_num_threads(std::thread::hardware_concurrency()) {}
    ThreadPool(uint32_t n_threads) : m_num_threads(n_threads) {}

    void start();
    void queue_job(const std::function<void()> &job);
    void stop();

private:
    void thread_loop();

    // Tells threads to stop looking for jobs
    bool m_should_terminate = false;

    // Prevents data races to the job queue
    std::mutex m_queue_mutex;

    // Allows threads to wait on new jobs or termination 
    std::condition_variable m_mutex_condition;

    std::queue<std::function<void()>> m_jobs;
    std::vector<std::thread> m_threads;

    uint32_t m_num_threads;
};

#endif //THREAD_POOL_H