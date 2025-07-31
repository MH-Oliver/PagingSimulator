// ===== Modified File: src/des/Event.cpp =====
#include "des/Event.h"

Event::Event(std::function<void()> cb, double time)
    : callback(std::move(cb)), timestamp(time) {}

double Event::getTimestamp() const {
    return timestamp;
}

void Event::processEvent() {
    callback();
}
