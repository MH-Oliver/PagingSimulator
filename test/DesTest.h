#ifndef DESTEST_H
#define DESTEST_H
#include "des/Des.h"

using namespace std;

int generateRandomNumber() {
    int randomNumber = rand() % 20 + 1;
    return randomNumber;
}

EventQueue q;

queue<int> buffer;
int bufferMax = 5;

int consumerTime = generateRandomNumber();
int producerTime = generateRandomNumber();
int amountProduced = 0;
int actAmountProduced = 0;
int maxTime = 100;
int maxProduced = 20;

void produce() {
    int val = rand() % 20;

    if(amountProduced > maxProduced) {
        cout << "Max Products reached" << endl;
    } else {
        buffer.push(val);
        cout << "Inserted val: " << val << endl;
        amountProduced++;
    }

}

void consume() {
    if(buffer.size() > 0) {
        cout << "read value: " << buffer.front() << endl;
        buffer.pop();
    } else {
        cout << "Buffer empty" << endl;
    }
}

void test_1(){
    cout << "Test 1:" << endl;
}


void test_2(){
    cout << "Test 2:" << endl;
    Event *g = new Event(&test_1, producerTime);
    q.AddEvent(g);
}


void test_3(){
    cout << "Test 3:" << endl;
}

class DesTest {

public:
    static void executeTests() {
        cout << "--- Test vom DES ---" << endl;

        srand(time(0));

        /*
        while (producerTime <= maxTime) {

                if (buffer.size() < bufferMax) {
                    Event *e = new Event(&produce, producerTime);
                    q.AddEvent(e);
                    actAmountProduced++;
                }
                else {
                    std::cout << "buffer is full." << std::endl;
                }
                producerTime += generateRandomNumber();

        }

        while (consumerTime <= maxTime) {
            if (buffer.size() < bufferMax) {
                Event *e = new Event(&consume, consumerTime);
                q.AddEvent(e);
                actAmountProduced++;
            }
            else {
                std::cout << "buffer is full." << std::endl;
            }
            consumerTime += generateRandomNumber();
        }

        */

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

#endif //DESTEST_H
