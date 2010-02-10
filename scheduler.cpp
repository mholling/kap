#include "scheduler.h"
#include "critical_section.h"

void Scheduler::Task::operator ()() {
  CriticalSection cs;
  if (enqueue()) // TODO: qualifier needed?
    if (at_head())
      start();
}

void Scheduler::Task::start() {
  sei();
  run();
  cli();
  dequeue();
  if (any()) head().start();
}
