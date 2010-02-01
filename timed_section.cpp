#include "timed_section.h"
#include "app.h"
#include "serial.h"

TimedSection::TimedSection(const char * const s) : start(TCNT2), s(s) { }

TimedSection::~TimedSection() {
  const int finish = TCNT2;
  const float time = static_cast<float>(finish - start) / (OCR2A * Timer::frequency);
  app.serial.debug(s, time);
}