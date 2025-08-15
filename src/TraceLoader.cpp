/**
* @file TraceLoader.cpp
 * @brief Implementation of trace file loading and scheduling.
 */
#include "TraceLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
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
    double t = startTime;

    while (std::getline(file, line)) {
        // Trim leading whitespace
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
        if (trimmed.empty() || trimmed[0] == '#') continue;

        std::istringstream iss(trimmed);
        int  pageId; char rw = 'R';
        iss >> pageId;
        if (iss.good()) iss >> rw;
        bool write = (rw == 'W' || rw == 'w');

        Event* e = new Event([sim, pageId, write]() {
            MemoryAccessEvent ev(pageId, write);
            sim->handleMemoryAccess(ev);
        }, t);

        eq.AddEvent(e);
        t += delta;
    }
}
