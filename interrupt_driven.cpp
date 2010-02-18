#include "interrupt_driven.h"
#include "safe.h"

bool InterruptDriven::Item::enqueue(bool block) volatile {
  bool success = Safe<Item>(this)->enqueue();
  if (success && block) wait();
  return success;
}

bool InterruptDriven::Item::enqueue() {
  if (!Queable<Item>::enqueue()) return false;
  after_enqueue();
  if (at_head()) initiate();
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

