#ifndef __DIAGNOSTIC_TASK_H_
#define __DIAGNOSTIC_TASK_H_

#include "scheduler.h"
#include "Eeprom.h"

class DiagnosticTask : public Scheduler::Task {
public:
  DiagnosticTask() : Scheduler::Task(0) {}
  void run();
};

#endif
