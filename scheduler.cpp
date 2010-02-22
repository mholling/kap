#include "scheduler.h"
#include "critical_section.h"
#include <avr/io.h>

bool Scheduler::Task::enqueue() {
  CriticalSection cs;
  if (!Base::enqueue()) return false;
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
  Base::dequeue();
  if (any()) head().start();
}
