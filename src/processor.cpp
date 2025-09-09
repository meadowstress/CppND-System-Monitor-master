#include "processor.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<float> cpu_buffer = LinuxParser::CpuUtilization();
  utilization_ = cpu_buffer.front();
  return (utilization_);
}