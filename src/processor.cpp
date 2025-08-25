#include "processor.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  std::string line;
  std::getline(stream, line);  // Read the first line

  std::istringstream ss(line);
  std::string cpu;
  ss >> cpu;  // Skip the "cpu" part

  long user, nice, system, idle, iowait, irq, softirq, steal;
  ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

  // Calculate total and active time
  long total_time =
      user + nice + system + idle + iowait + irq + softirq + steal;
  long active_time = user + nice + system + irq + softirq + steal;

  float cpu_load = (static_cast<float>(active_time) / total_time);

  return (cpu_load);  // Return CPU utilization percentage
}