/**
* @file EventQueue.cpp
 * @brief Implementation of the DES priority queue.
 */

#include "des/EventQueue.h"
#include "des/Event.h"

EventQueue::EventQueue() = default;
EventQueue::~EventQueue() = default;
EventQueue::EventQueue(EventQueue&&) noexcept = default;
EventQueue& EventQueue::operator=(EventQueue&&) noexcept = default;

bool EventQueue::Cmp::operator()(const std::unique_ptr<Event>& a,
                                 const std::unique_ptr<Event>& b) const
{
    return a->time() > b->time();
}

void EventQueue::AddEvent(Event* e)
{
    if (!e) return;
    pq_.emplace(e);
}

void EventQueue::run()
{
    while (!pq_.empty()) {
        step();
    }
}

void EventQueue::step()
{
    if (pq_.empty()) return;

    std::unique_ptr<Event> ev = std::move(const_cast<std::unique_ptr<Event>&>(pq_.top()));
    pq_.pop();

    ev->run();
}

void EventQueue::clear()
{
    while (!pq_.empty()) pq_.pop();
}
