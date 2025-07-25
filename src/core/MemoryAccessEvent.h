#ifndef MEMORYACCESSEVENT_H
#define MEMORYACCESSEVENT_H



class MemoryAccessEvent {
private:
    int pageId;

public:
    MemoryAccessEvent(int id) : pageId(id) {}
    int getPageId() const { return pageId; }
};



#endif //MEMORYACCESSEVENT_H
