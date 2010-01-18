#include "scheduler.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "critical_section.h"

void Scheduler::init() {
  sei();
}

void Scheduler::run_tasks(Task* new_task) {
  CriticalSection cs;
  if (new_task) new_task->enqueue();
  volatile Task* interrupted_task = current_task;
  while (Task::any()) {
    Task& task = Task::head();
    current_task = &task;
    if (current_task == interrupted_task) break;
    sei();
    task();
    cli();
    task.complete(); // TODO: could this functionality be moved into the Task class as per SPI?
  }
  current_task = interrupted_task;
}

void Scheduler::idle() {
  // // can't idle for now because it triggers ADC conversion and screws everything up... (TODO!)
  // set_sleep_mode(SLEEP_MODE_IDLE);
  // sleep_mode();
}

void Scheduler::run() {
  while (true) {
    run_tasks();
    idle();
  }
}

void Scheduler::signal(Task& task) {
  run_tasks(&task);
}
