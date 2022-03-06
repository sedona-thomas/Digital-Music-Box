#include "ValueQueue.h"

ValueQueue::ValueQueue() {
  for (int i = 0; i < 5; i++) {
    values.push_back(0);
  }
}

ValueQueue::ValueQueue(int size) {
  for (int i = 0; i < size; i++) {
    values.push_back(0);
  }
}

void ValueQueue::add(uint8_t value) {
  push(value);
  pop();
}

inline void ValueQueue::push(uint8_t value) { values.push_back(value); }

inline void ValueQueue::pop() { values.pop_front(); }

bool ValueQueue::contains(uint8_t i) {
  return std::find(values.begin(), values.end(), i) != values.end();
}

uint8_t ValueQueue::average() {
  int sum = 0;
  for (auto val : values) {
    sum += val;
  }
  return sum / size();
}

uint8_t ValueQueue::median() {
  std::list<uint8_t> temp(values);
  temp.sort();
  return temp[temp.size()];
}

inline size_t ValueQueue::size() { return values.size(); }