#include "interrupt_driven.h"
#include "safe.h"

void InterruptDriven::Item::enqueue(bool block) volatile {
  { Safe<Item>(this)().enqueue(); }
  if (block) wait();
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

void InterruptDriven::Item::dequeue() {
  Queable<Item>::dequeue();
  if (any()) head().initiate();
}

