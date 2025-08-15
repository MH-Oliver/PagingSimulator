/**
* @file EventQueue.cpp
 * @brief Implementation of the DES priority queue.
 */

#include "des/EventQueue.h"
#include "des/Event.h"   // 여기서 Event를 완전형으로 포함

// --- 수명 관련 special members를 TU로 이동(여기서 default) ---
EventQueue::EventQueue() = default;
EventQueue::~EventQueue() = default;
EventQueue::EventQueue(EventQueue&&) noexcept = default;
EventQueue& EventQueue::operator=(EventQueue&&) noexcept = default;

// --- Comparator 정의 (Event::time() 사용 가능) ---
bool EventQueue::Cmp::operator()(const std::unique_ptr<Event>& a,
                                 const std::unique_ptr<Event>& b) const
{
    // priority_queue는 기본 max-heap → time이 작은 것이 위로 오도록 반전
    return a->time() > b->time();
}

// --- Public API ---
void EventQueue::AddEvent(Event* e)
{
    if (!e) return;
    pq_.emplace(e); // raw 포인터 소유권을 unique_ptr로 감싸서 큐에 저장
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

    // 가장 이른 이벤트 꺼내기
    std::unique_ptr<Event> ev = std::move(const_cast<std::unique_ptr<Event>&>(pq_.top()));
    pq_.pop();

    // 실행 후 unique_ptr 범위 종료 시 자동 파괴
    ev->run();
}

void EventQueue::clear()
{
    while (!pq_.empty()) pq_.pop(); // 내부 unique_ptr들이 파괴되며 Event도 삭제됨
}
