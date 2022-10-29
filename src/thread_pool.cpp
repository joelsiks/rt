
#include "thread_pool.h"

void ThreadPool::thread_loop() {
    while(true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_mutex_condition.wait(lock, [this] {
                return !m_jobs.empty() || m_should_terminate;
            });

            if(m_should_terminate && m_jobs.empty()) {
                return;
            }

            job = std::move(m_jobs.front());
            m_jobs.pop();
        }

        job();
    }
}

void ThreadPool::start() {
    m_threads.resize(m_num_threads);
    for(uint32_t i = 0; i < m_num_threads; i++) {
        m_threads.at(i) = std::thread(&ThreadPool::thread_loop, this);
    }
}

void ThreadPool::queue_job(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_jobs.push(job);
    }

    m_mutex_condition.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_should_terminate = true;
    }

    m_mutex_condition.notify_all();

    for(std::thread& active_thread : m_threads) {
        active_thread.join();
    }

    m_threads.clear();
}