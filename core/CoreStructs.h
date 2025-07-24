#ifndef CORESTRUCTS_H
#define CORESTRUCTS_H

/**
 * Repräsentiert einen einzelnen Rahmen im physischem Speicher
 */
struct PageFrame{
    int pageId;
    bool dirtyBit;      // Wurde Rahmen verändert?
    bool referencedBit; // Kürzlicher Zugriff?
};

struct PageTableEntry {
    bool isPresent;   // Ist die Seite im Hauptspeicher?
    int frameIndex;   // Welcher physischer Rahmen enthält die Seite (-1, wenn nicht präsent)
};

struct PageTable {
    std::vector<PageTableEntry> entries;
};

struct TLBEntry {
    unsigned int page_index;
    unsigned int page_frame_index;
    unsigned char frame_attributes;
};

struct TLB {
    std::vector<TLBEntry> entries;
    unsigned int size;
};

struct Process {
    unsigned char process_id;
    PageTable *page_table;
};

struct MMU {
    std::unique_ptr<TLB> tlb;
    std::unique_ptr<Process> process;
};

#endif //CORESTRUCTS_H
