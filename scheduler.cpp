#include "scheduler.h"
#include "critical_section.h"

void Scheduler::Task::operator ()() {
  CriticalSection cs;
  if (PrioritisedQueable<Task>::enqueue())
    if (at_head())
      start();
}

void Scheduler::Task::start() {
  if (!started) {
    started = true;
    sei();
    run();
    cli();
    started = false;
    PrioritisedQueable<Task>::dequeue();
  }
  if (any()) head().start();
}
