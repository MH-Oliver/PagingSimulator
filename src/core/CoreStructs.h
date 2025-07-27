#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H
#include <iostream>
using namespace std;
/**
 * Repräsentiert einen einzelnen Rahmen im physischem Speicher
 */
struct PageFrame{
    int pageId;
    bool dirtyBit;      // Wurde Rahmen verändert
    bool referencedBit; // Kürzlicher Zugriff
    long long lastAccessTime;
    long long loadTime;
    int accessCounter;
};

struct PageTableEntry {
    bool isPresent;
    int frameIndex; // Index des Rahmens zu der aktuellen Seite
};

struct PageTable {
    std::vector<PageTableEntry> entries;

    PageTable(unsigned int numVirtualPages) {
        entries.resize(numVirtualPages);
        for (PageTableEntry& entry : entries) {
            entry.isPresent = false;
            entry.frameIndex = -1;
        }
    }
};

struct TLBEntry {
    unsigned int page_index;
    unsigned int page_frame_index;
    unsigned char frame_attributes;
    // Hier könnten auch Attribute für die Paging-Algorithmen gespeichert werden
};

struct TLB {
    std::vector<TLBEntry> entries;
    unsigned int capacity; // Maximale Anzahl von Einträgen in der TLB

    TLB(unsigned int cap) : capacity(cap) {
        entries.reserve(cap);
    }

    void clear() {
        entries.clear();
    }

    /**
     * Zum Prüfen, ob eine Seite bereits einem Rahmen zugeordnet ist.
     * @param pageIndex Seite für die der zugehörige Rahmen gesucht wird.
     * @return Falls die Seite gefunden wurde, wird der entsprechende Rahmen zurückgegeben. Sonst -1.
     */
    int lookup(int pageIndex) {
        for (const TLBEntry& entry : entries) {
            if (entry.page_index == pageIndex) {
                return entry.page_frame_index;
            }
        }
        return -1;
    }

    void addEntry(int pageIndex, int frameIndex) {
        if (entries.size()+1 <= capacity) {
            entries.push_back(TLBEntry(pageIndex,frameIndex));
        } else {
            cout << "  TLB hat keine Kapazität mehr. Es findet kein neuer Eintrag statt." << endl;
        }
    }

    void deleteEntry(int victim_frame_index) {
        for (int i = 0; i < entries.size(); i++) {
            const TLBEntry& entry = entries[i];
            if (entry.page_frame_index == victim_frame_index) {
                entries.erase(entries.begin() + i);
            }
        }
    }

    int getPageForFrame(int victim_frame_index) {
        for (const TLBEntry& entry : entries) {
            if (entry.page_frame_index == victim_frame_index) {
                return entry.page_index;
            }
        }
        return -1;
    }
};

struct Process {
    unsigned char process_id;
    PageTable page_table;

    Process(unsigned char id, unsigned int numVirtualPages)
        : process_id(id), page_table(numVirtualPages) {}
};

struct MMU {
    TLB tlb;
    Process* currentProcess;

    MMU(unsigned int tlbCapacity) : tlb(tlbCapacity), currentProcess(nullptr) {}

    void setCurrentProcess(Process* p) {
        currentProcess = p;
        tlb.clear();
    }
};

#endif //CORESTRUCTS_H
