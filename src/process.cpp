#include "process.h"

#include <unistd.h>

#include <cctype>
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

  const float Hertz =
      100.0F;  // constant taken from bash commandline via getconf CLK_TCK
  float total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  float seconds = std::abs(utime - (starttime / Hertz));
  float cpu_usage = (total_time / (Hertz * seconds));

  cpu_utilization_ = cpu_usage;

  return cpu_usage;
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
  user_ = LinuxParser::User(std::stoi(LinuxParser::Uid(pid_)));
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  up_time_ = LinuxParser::UpTime(pid_);
  return up_time_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  bool result{false};
  if (this->ram_ < a.ram_) {
    result = true;
  } else {
    result = false;
  }
  return result;
}