//
// Created by shakhov on 3/29/20.
//

#ifndef CONCURRENCY_SYNCH_QUEUE_H
#define CONCURRENCY_SYNCH_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

template <typename T>
class SynchQueue {

    std::deque<T> queue;
    mutable std::mutex m;
    std::condition_variable cv;
    size_t counter = 0;
    size_t mal = -1;
public:
    void set_mal(const size_t& value) {
        std::lock_guard<std::mutex> {m};
        mal = value;
    }

    size_t size() { return queue.size(); }

    size_t get_counter() {
        std::lock_guard<std::mutex> {m};
        return counter;
    }

    void increase_count() {
        std::lock_guard<std::mutex> {m};
        counter += 1;
    }

    bool check_end() {
        std::lock_guard<std::mutex> {m};
        return counter == mal;
    }

    void discrease_max() {
        std::lock_guard<std::mutex> {m};
        mal -= 1;
    }

    T pop() {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this]{ return !queue.empty(); });
        T elem = queue.front();
        queue.pop_front();
        return elem;
    }

    std::pair<T, T> pop2() {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this]{ return queue.size() >= 2; });
        T elem1 = queue.front();
        queue.pop_front();
        T elem2 = queue.front();
        queue.pop_front();
        return std::pair(elem1, elem2);
    }

    void push(const T& value) {
        {
            std::lock_guard<std::mutex>{m};
            queue.push_back(value);
        }
        cv.notify_one();
    }

    void push2(const T& value, const T& value2) {
        {
            std::lock_guard<std::mutex>{m};
            queue.push_front(value);
            queue.push_front(value2);
        }
        cv.notify_one();
    }
    void push_front(const T& value) {
        {
            std::lock_guard<std::mutex>{m};
            queue.push_front(value);
        }
        cv.notify_one();
    }

    void push(T&& value) {
        {
            std::lock_guard<std::mutex>{m};
            queue.push_back(value);
        }
        cv.notify_one();
    }

    template <typename ...Args>
    void emplace(Args&& ...args) {
        {
            std::lock_guard<std::mutex>{m};
            queue.emplace_back(args...);
        }
        cv.notify_one();
    }

    bool empty() const {
        std::lock_guard<std::mutex>{m};
        return queue.empty();
    }
};


#endif //CONCURRENCY_SYNCH_QUEUE_H
