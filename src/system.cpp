#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() {
  // for (int i = 0; i < 20; i++) {
  //   std::cout << std::endl;
  // }
  // std::cout << "\n CPU. = " << cpu_.Utilization() << std::endl;
  return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  // for (int i = 0; i < 1; i++) {
  //   std::cout << std::endl;
  // }
  // std::cout << "\nRunningProcesses = " << LinuxParser::RunningProcesses()
  //           << std::endl;
  return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  // for (int i = 0; i < 30; i++) {
  //   std::cout << std::endl;
  // }
  // std::cout << "\nTotalProcesses = " << LinuxParser::TotalProcesses()
  //           << std::endl;
  return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  // for (int i = 0; i < 1; i++) {
  //   std::cout << std::endl;
  // }
  // std::cout << "\nUpTime = " << LinuxParser::UpTime() << std::endl;
  return LinuxParser::UpTime();
}
