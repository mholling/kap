#include "scheduler.h"

void Scheduler::Task::operator ()() {
  if (enqueue())
    if (at_head()) start();
}

void Scheduler::Task::start() {
  const unsigned char sreg = SREG;
  sei();
  run();
  SREG = sreg;
  dequeue();
}

void Scheduler::Task::dequeue() {
  PrioritisedQueable<Task>::dequeue();
  if (any()) head().start();
}
