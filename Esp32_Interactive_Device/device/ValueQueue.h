#ifndef __VALUE_QUEUE_H__
#define __VALUE_QUEUE_H__

#include <cstdlib>
#include <list>
#include <stdint.h>
#include <string>

class ValueQueue {
private:
  std::list<uint8_t> values;

public:
  ValueQueue();
  ValueQueue(int);
  inline void add(uint8_t value);
  void push(uint8_t value);
  void pop();
  inline uint8_t contains(int i);
  uint8_t average();
  inline size_t size();
};

#endif