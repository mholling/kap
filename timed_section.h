#ifndef __TIMED_SECTION_H_
#define __TIMED_SECTION_H_

class TimedSection {
  const int start;
  const char * const s;
public:
  TimedSection(const char * const s);
  ~TimedSection();
};

#endif
