#include "scheduler.h"
#include <avr/interrupt.h>
#include "app.h"

void Scheduler::init() {
  sei();
}

void Scheduler::run_at_higher_level() {
  level++;  
  while (Task::any()) {
    Task& task = Task::head();
    if (task.level < level) break;
    task();
    task.dequeue();
  }
  level--;
}

void Scheduler::run() {
  // eventually this will just set idle mode!
  while (true) {
    delay(1000);
    App::app().magnetometer.show_status();
    App::app().magnetometer.show_bearing();
  }
}

void Scheduler::signal(Task& task) {
  task.enqueue();
  sei();
  run_at_higher_level();
}
