// src/des/Event.h
/**
 * @file Event.h
 * @brief Discrete event with a scheduled time and callable action.
 */
#ifndef DES_EVENT_H
#define DES_EVENT_H

#include <functional>
#include <utility>  // for std::move

/**
 * @brief Simple event wrapper for the DES.
 */
class Event {
public:
  using Action = std::function<void()>;

  /**
   * @brief Construct an event.
   * @param a Action to execute.
   * @param t Simulation time at which to execute.
   */
  Event(Action a, double t) : action_(std::move(a)), time_(t) {}

  /** @brief Execute the action. */
  void run() { action_(); }

  /** @return Scheduled time of the event. */
  double time() const { return time_; }

private:
  Action action_; ///< Callback to execute.
  double time_;   ///< Scheduled time.
};

#endif // DES_EVENT_H
