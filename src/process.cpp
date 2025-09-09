#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  float utime{}, stime{}, cutime{}, cstime{}, starttime{};
  string line{}, buffer{};
  std::vector<int> data{};

  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid_) +
                       LinuxParser::kStatFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);

    for (int i = 1; i <= 13; i++) {
      linestream >> buffer;
    }

    linestream >> utime >> stime >> cutime >> cstime;

    for (int i = 18; i <= 21; i++) {
      linestream >> buffer;
    }

    linestream >> starttime;
  }

  long Hertz = sysconf(_SC_CLK_TCK);

  if (Hertz == -1) {
    std::cerr << "Error retrieving clock ticks per second: " << errno
              << std::endl;
    return 1.F;
  }

  float system_up_time = static_cast<float>(LinuxParser::UpTime());
  float total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  float seconds = system_up_time - (starttime / Hertz);

  cpu_utilization_ = (total_time / (Hertz * seconds));

  return cpu_utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() {
  command_ = LinuxParser::Command(pid_);
  return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() {
  user_ = LinuxParser::User(LinuxParser::Uid(pid_));
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  up_time_ = LinuxParser::UpTime(pid_);
  return up_time_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  bool result{false};
  if (this->cpu_utilization_ < a.cpu_utilization_) {
    result = true;
  } else {
    result = false;
  }
  return result;
}