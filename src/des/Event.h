#ifndef UE2_NEW_EVENT_H
#define UE2_NEW_EVENT_H

#include <functional>

class Event {
private:
    double timestamp;
    std::function<void()> callback;

public:
    Event(std::function<void()> cb, double time);
    double getTimestamp() const;
    void processEvent();
};

#endif // UE2_NEW_EVENT_H