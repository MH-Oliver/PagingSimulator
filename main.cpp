#include "Simulation.h"
#include "des/Des.h"
#include "core/CoreStructs.h"

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

int main() {
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

    cout << "--- Test von handleMemoryAccess() ---" << endl;

    const int NUM_FRAMES = 4;
    const int TLB_CAPACITY = 2;
    const int NUM_VIRTUAL_PAGES_PROCESS1 = 10;

    Simulation simulation(NUM_FRAMES, nullptr, TLB_CAPACITY);
    Process process1(1, NUM_VIRTUAL_PAGES_PROCESS1);
    simulation.setCurrentProcess(&process1);

    std::cout << "\n--- Testfall 1: Erster Zugriff (Page Fault) ---" << std::endl;
    MemoryAccessEvent event1(0);
    simulation.handleMemoryAccess(&event1);
    //TODO folgender Test(1 und 3) muss übearbeitet werden, wenn PageFault implementiet (auch addTlbEntry().
    simulation.addTlbEntry(event1.getPageId(), 0);

    std::cout << "\n--- Testfall 2: Zugriff auf gleiche Seite (Page Hit / TLB Hit erwartet) ---" << std::endl;
    // Seite 0 ist jetzt in Frame 0 und sollte auch in der TLB sein.
    MemoryAccessEvent event2(0);
    simulation.handleMemoryAccess(&event2);

    std::cout << "\n--- Testfall 3: Zugriff auf neue Seite (Page Fault, füllt TLB) ---" << std::endl;
    // Seite 1 ist nicht im Speicher.
    MemoryAccessEvent event3(1);
    simulation.handleMemoryAccess(&event3);
    simulation.addTlbEntry(event3.getPageId(), 1);

    //TODO Implementierung für Fall wenn TLB voll
    std::cout << "\n--- Testfall 4: Zugriff auf dritte Seite (Page Fault, TLB überläuft, Seite 0 wird verdrängt) ---" << std::endl;
    // Seite 2 ist nicht im Speicher. TLB-Kapazität ist 2. Seite 0 sollte verdrängt werden.
    MemoryAccessEvent event4(2);
    simulation.handleMemoryAccess(&event4);


}