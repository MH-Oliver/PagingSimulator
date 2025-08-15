/**
* @file TraceLoader.h
 * @brief Load a trace file and schedule memory access events.
 */
#ifndef TRACELOADER_H
#define TRACELOADER_H

#include <string>
#include "des/EventQueue.h"
#include "Simulation.h"

/**
 * @brief Load a trace of page accesses and schedule them into the event queue.
 * @details Each line: "pageId [R|W]". Lines starting with '#' are ignored.
 * @param filename Path to the trace file.
 * @param eq Event queue to schedule into (takes ownership of created events).
 * @param sim Simulation to call when events fire.
 * @param startTime Time of first event.
 * @param delta Time spacing between events.
 */
void loadTrace(const std::string& filename,
               EventQueue& eq,
               Simulation* sim,
               double startTime = 1.0,
               double delta = 1.0);

#endif // TRACELOADER_H
