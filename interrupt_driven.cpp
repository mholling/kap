#include "interrupt_driven.h"
#include "safe.h"

bool InterruptDriven::Item::enqueue(bool block) volatile {
  bool success;
  { success = Safe<Item>(this)().enqueue(); }
  if (success && block) wait();
  return success;
}

bool InterruptDriven::Item::enqueue() {
  bool success = Queable<Item>::enqueue();
  if (success && at_head()) initiate();
  return success;
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

