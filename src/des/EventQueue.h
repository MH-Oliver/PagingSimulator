/**
 * @file EventQueue.h
 * @brief Discrete-Event Simulation (DES) priority queue.
 *
 * This queue stores events ordered by their scheduled time (ascending).
 * It owns the events; when an event is executed via step() or run(),
 * the event object is destroyed automatically.
 *
 * Typical usage:
 * @code{.cpp}
 * EventQueue q;
 * q.AddEvent(new Event([](){}, 1.0)); // do something at t=1.0
 * q.AddEvent(new Event([](){}, 2.0)); // do something at t=2.0
 * q.step(); // executes the earliest (time=1.0)
 * q.run();  // executes the rest
 * @endcode
 */

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <memory>
#include <queue>
#include <vector>
#include <cstddef>

class Event; // forward declaration (정의는 .cpp에서 include)

class EventQueue {
public:
    EventQueue();                       // 정의는 .cpp
    ~EventQueue();                      // 정의는 .cpp  ← 중요: 소멸자를 TU로 이동
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    EventQueue(EventQueue&&) noexcept;        // 정의는 .cpp
    EventQueue& operator=(EventQueue&&) noexcept; // 정의는 .cpp

    /// Add an event (takes ownership).
    void AddEvent(Event* e);            // 정의는 .cpp
    /// Execute all remaining events.
    void run();                         // 정의는 .cpp
    /// Execute the next earliest event (if any).
    void step();                        // 정의는 .cpp
    /// Remove all pending events without executing.
    void clear();                       // 정의는 .cpp

    bool empty() const { return pq_.empty(); }
    std::size_t size() const { return pq_.size(); }

private:
    /// Comparator for min-heap by event time (정의는 .cpp)
    struct Cmp {
        bool operator()(const std::unique_ptr<Event>& a,
                        const std::unique_ptr<Event>& b) const;
    };

    using Ptr = std::unique_ptr<Event>;

    // Min-heap: the smallest time on top.
    std::priority_queue<
        Ptr,
        std::vector<Ptr>,
        Cmp
    > pq_;
};

#endif // EVENTQUEUE_H
