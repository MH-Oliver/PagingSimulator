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

class Event;

class EventQueue {
public:
    EventQueue();
    ~EventQueue();
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    EventQueue(EventQueue&&) noexcept;
    EventQueue& operator=(EventQueue&&) noexcept;

    /// Add an event (takes ownership).
    void AddEvent(Event* e);
    /// Execute all remaining events.
    void run();
    /// Execute the next earliest event (if any).
    void step();
    /// Remove all pending events without executing.
    void clear();

    bool empty() const { return pq_.empty(); }
    std::size_t size() const { return pq_.size(); }

private:
    /// Comparator for min-heap by event time
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
