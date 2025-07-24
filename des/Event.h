//
// Created by Markus Springer on 25.03.16.
//

#ifndef UE2_NEW_EVENT_H
#define UE2_NEW_EVENT_H

#include <iostream>
#include <ctime>

class Event {
private:
    int timestamp;
    void (*function)();

public:
    Event( void (*called_function)(), int time) {
        timestamp = time;
        function = called_function;
    }

    int getTimestamp() const {
        return timestamp;
    }

    void processEvent();
};


#endif //UE2_NEW_EVENT_H
