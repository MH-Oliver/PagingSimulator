// src/test/DesTest.h
/**
 * @file DesTest.h
 * @brief Small inline test driver for the DES queue.
 */
#ifndef DESTEST_H
#define DESTEST_H

#include <iostream>
#include <queue>
#include <cstdlib> // rand, srand
#include <ctime>   // time
#include "des/Des.h"

inline int generateRandomNumber() {
    return std::rand() % 20 + 1;
}

inline EventQueue q;

inline std::queue<int> buffer;
inline int bufferMax = 5;

inline int consumerTime;
inline int producerTime;
inline int amountProduced = 0;
inline int actAmountProduced = 0;
inline int maxTime = 100;
inline int maxProduced = 20;

inline void produce() {
    int val = std::rand() % 20;
    if (amountProduced > maxProduced) {
        std::cout << "Max Products reached\n";
    } else {
        buffer.push(val);
        std::cout << "Inserted val: " << val << "\n";
        amountProduced++;
    }
}

inline void consume() {
    if (!buffer.empty()) {
        std::cout << "read value: " << buffer.front() << "\n";
        buffer.pop();
    } else {
        std::cout << "Buffer empty\n";
    }
}

inline void test_1(){ std::cout << "Test 1:\n"; }
inline void test_2(){
    std::cout << "Test 2:\n";
    Event *g = new Event(&test_1, producerTime);
    q.AddEvent(g);
}
inline void test_3(){ std::cout << "Test 3:\n"; }

/**
 * @brief Execute a minimal sequence to validate DES behavior.
 */
class DesTest {
public:
    static void executeTests() {
        std::cout << "--- DES quick test ---\n";
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        producerTime = generateRandomNumber();
        consumerTime = generateRandomNumber();

        Event *e = new Event(&test_1, producerTime);
        Event *f = new Event(&test_2, producerTime);
        Event *g = new Event(&test_3, producerTime);
        q.AddEvent(e);
        q.AddEvent(f);
        q.AddEvent(g);

        q.step();
        q.step();
    }
};

#endif // DESTEST_H
