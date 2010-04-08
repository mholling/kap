#include "task.h"
#include "critical_section.h"
#include <avr/io.h>

bool Task::enqueue() {
  CriticalSection cs;
  if (!Base::enqueue()) return false;
  if (at_head()) start();
  return true;
}

void Task::start() {
  sei();
  run();
  cli();
  dequeue();
}

void Task::dequeue() {
  Base::dequeue();
  if (any()) head().start();
}
