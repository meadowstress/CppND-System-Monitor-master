#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  uint hh = seconds / 3600;
  uint mm = (seconds % 3600) / 60;
  uint ss = (seconds % 3600) % 60;
  string result =
      std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);
  return result;
}