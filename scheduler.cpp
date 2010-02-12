#include "scheduler.h"

void Scheduler::Task::operator ()() {
  if (enqueue())
    if (at_head()) start();
}

void Scheduler::Task::start() {
  sei();
  run();
  cli();
  dequeue();
}

void Scheduler::Task::dequeue() {
  PrioritisedQueable<Task>::dequeue();
  if (any()) const_cast<Task&>(head()).start();
}
