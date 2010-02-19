#include "interrupt_driven.h"
#include "critical_section.h"

bool InterruptDriven::Item::enqueue(bool block) {
  { // TODO: change this to operator () ?
    CriticalSection cs;
    if (!Queable<Item>::enqueue()) return false;
    after_enqueue();
    if (at_head()) initiate();
  }
  if (block) wait();
  return true;
}

void InterruptDriven::Item::interrupt() {
  if (process()) {
    terminate();
    dequeue();
  }
}

void InterruptDriven::interrupt() {
  Item::head().interrupt();
}

void InterruptDriven::Item::dequeue() {
  Queable<Item>::dequeue();
  if (any()) head().initiate();
}

