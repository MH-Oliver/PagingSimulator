#include "TraceLoader.h"
#include <fstream>
#include <iostream>
#include <string>
#include "core/MemoryAccessEvent.h"
#include "des/Event.h"

void loadTrace(const std::string& filename,
               EventQueue& eq,
               Simulation* sim,
               double startTime,
               double delta) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open trace file: " << filename << std::endl;
        return;
    }

    std::string line;
    double time = startTime;
    // Datei zeilenweise einlesen
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        int pageId = std::stoi(line);
        // Event mit Lambda planen
        Event* e = new Event(
            [sim, pageId]() {
                MemoryAccessEvent ev(pageId);
                sim->handleMemoryAccess(&ev);
            },
            time
        );
        eq.AddEvent(e);
        time += delta;
    }
}