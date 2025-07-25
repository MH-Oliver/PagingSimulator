#ifndef UE2_NEW_EVENTQUEUE_H
#define UE2_NEW_EVENTQUEUE_H
#include <vector>
#include <queue>
#include "Event.h"

struct CompareNode : public std::binary_function<Event*, Event*, bool> {
    bool operator()(const Event* lhs, const Event* rhs) const
    {
        return lhs->getTimestamp() > rhs->getTimestamp();
    }
};

class EventQueue {
private:
    bool pause = false;
    bool stopped = false;
public:
    std::priority_queue<Event*,std::vector<Event*>, CompareNode > eq;
    EventQueue() {
    };

    void AddEvent(Event *e);

    bool supervisorStopped();
    bool isPaused();
    void step();
    void run();
    long getSize();

};


#endif //UE2_NEW_EVENTQUEUE_H
