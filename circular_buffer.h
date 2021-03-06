#ifndef __CIRCULAR_BUFFER_H_
#define __CIRCULAR_BUFFER_H_

template <typename T, int N>
class CircularBuffer {
private:
  T buffer[N];
  int start, end;
  
public:
  CircularBuffer() : start(0), end(0) { }
  
  bool empty() const { return start == end; }
  bool any() const { return start != end; }

  CircularBuffer& operator <<(const T& t) {
    buffer[end] = t;
    ++end %= N;
    if (end == start) ++start %= N;
    return *this;
  }
    
  CircularBuffer& operator >>(volatile T& t) {
    if (empty())
      t = 0;
    else {
      t = buffer[start];
      ++start %= N;
    }
    return *this;
  }
};

#endif
