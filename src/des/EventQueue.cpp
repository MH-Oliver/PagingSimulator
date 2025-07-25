//
// Created by Markus Springer on 25.03.16.
//

#include "EventQueue.h"

bool EventQueue::isPaused() {
    return this->pause;
}

void EventQueue::AddEvent(Event *e) {
    this->eq.push(e);
}

void EventQueue::step() {

    if(!this->isPaused() && !this->eq.empty()) {
        Event *n = this->eq.top();
        n->processEvent();
        this->eq.pop();
    }
}

void EventQueue::run() {
    while(!this->isPaused() && !this->eq.empty()) {
        Event *n = this->eq.top();

        n->processEvent();
        this->eq.pop();
    }
}

bool EventQueue::supervisorStopped() {
    return stopped;
}

long EventQueue::getSize(){
    return this->eq.size();
}