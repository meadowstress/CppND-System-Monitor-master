#include "format.h"

#include <iomanip>
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
  std::ostringstream result;
  result << std::setw(2) << std::setfill('0') << hh << ":" << std::setw(2)
         << std::setfill('0') << mm << ":" << std::setw(2) << std::setfill('0')
         << ss;
  return result.str();
}