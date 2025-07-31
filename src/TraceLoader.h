// ===== Datei: src/TraceLoader.h =====
#ifndef TRACELOADER_H
#define TRACELOADER_H

#include <string>
#include "des/EventQueue.h"
#include "Simulation.h"

/**
 * Lädt ein Trace-File mit Page-IDs (eine pro Zeile) und plant MemoryAccessEvents.
 * @param filename  Pfad zur Trace-Datei (z.B. "resources/trace.txt").
 * @param eq        Referenz auf DES-EventQueue.
 * @param sim       Pointer auf Simulation-Instanz.
 * @param startTime Zeitstempel des ersten Events (in Sekunden).
 * @param delta     Zeitabstand zwischen Events (in Sekunden).
 */
void loadTrace(const std::string& filename,
               EventQueue& eq,
               Simulation* sim,
               double startTime = 1.0,
               double delta = 1.0);

#endif // TRACELOADER_H