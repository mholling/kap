#include "scheduler.h"
#include "unsafe.h"

void Scheduler::Task::operator ()() {
  if (enqueue())
    if (at_head()) start();
}

void Scheduler::Task::start() {
  Unsafe<Task>(this)->run();
  dequeue();
}

void Scheduler::Task::dequeue() {
  PrioritisedQueable<Task>::dequeue();
  if (any()) head().start();
}
