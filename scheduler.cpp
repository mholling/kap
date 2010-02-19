#include "scheduler.h"
#include "critical_section.h"
#include <avr/io.h>

bool Scheduler::Task::enqueue() {
  CriticalSection cs;
  if (!PrioritisedQueable<Task>::enqueue()) return false;
  if (at_head()) start();
  return true;
}

void Scheduler::Task::start() {
  sei();
  run();
  cli();
  dequeue();
}

void Scheduler::Task::dequeue() {
  PrioritisedQueable<Task>::dequeue();
  if (any()) head().start();
}
