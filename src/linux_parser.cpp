#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line{};
  string key{};
  string value{};  // initializing value is essential!
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os{}, kernel{}, version{};  // init kernel is essential!
  string line{};
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids{};  // never forget to init!
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  uint mem_total{}, mem_free{}, mem_utilized{}, number{};
  string line{}, label{}, unit{};
  std::vector<uint> memory{};
  float result{};

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> number >> unit;
      memory.push_back(number);
    }

    const size_t kRelevantElement = 1u;
    if (kRelevantElement < memory.size()) {
      mem_total = memory.at(0);
      mem_free = memory.at(kRelevantElement);
    }

    mem_utilized = mem_total - mem_free;
    result = static_cast<float>(mem_utilized) / static_cast<float>(mem_total);
  }
  return result;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  float uptime{}, idle_time{};
  string line{};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_time;
  }
  float result = static_cast<long>(uptime);
  return result;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long total_jiffies{}, user{}, nice{}, system{}, idle{}, iowait{}, irq{},
      softirq{}, steal{};
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);  // Read the first line

    std::istringstream ss(line);
    std::string cpu;
    ss >> cpu;  // Skip the "cpu" part
    ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    // Calculate total and active time
    total_jiffies =
        user + nice + system + idle + iowait + irq + softirq + steal;
  }

  return (total_jiffies);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_time{}, user{}, nice{}, system{}, idle{}, iowait{}, irq{},
      softirq{}, steal{};
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);

  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);  // Read the first line
    std::istringstream ss(line);
    std::string cpu;
    ss >> cpu;  // Skip the "cpu" part
    ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    active_time = user + nice + system + irq + softirq + steal;
  }

  return (active_time);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_jiffies{}, idle{}, user{}, nice{}, system{}, iowait{}, irq{},
      softirq{}, steal{};
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);  // Read the first line
    std::istringstream ss(line);
    std::string cpu;
    ss >> cpu;  // Skip the "cpu" part
    ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  idle_jiffies = idle + iowait;
  return (idle_jiffies);
}

// TODO: Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization() {
  vector<float> cpu_utilization{};
  float cpu_overall_utilization =
      static_cast<float>(ActiveJiffies()) / static_cast<float>(Jiffies());
  cpu_utilization.push_back(cpu_overall_utilization);
  return (cpu_utilization);
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int num_processes{}, number{};
  string line{}, label{};
  std::vector<int> data{};

  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> number;
      data.push_back(number);
    }
    const size_t kRelevantElement = 8u;
    if (kRelevantElement < data.size()) {
      num_processes = data.at(kRelevantElement);
    }
  }
  return num_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int num_running_processes{}, number{};
  string line{}, label{}, unit{};
  std::vector<uint> data{};

  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> number >> unit;
      data.push_back(number);
    }

    const size_t kRelevantElement = 9u;
    if (kRelevantElement < data.size()) {
      num_running_processes = data.at(kRelevantElement);
    }
  }

  return (num_running_processes);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string result{}, number{}, line{}, label{};
  std::vector<string> data{};

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      result = line;
    }
  }
  return (result);
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string result{}, number{}, line{}, label{};
  std::vector<string> data{};

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> number;
      if (label == "VmRSS:") {
        result = number;
      }
    }
    if (result != "") {
      int conversion_kilo_to_megabyte =
          static_cast<int>(std::stof(result) / 1000.F);
      result = std::to_string(conversion_kilo_to_megabyte);
    }
  }
  return (result);
}

// TODO: Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
  string line{}, label{};
  int number{}, result{};
  // std::vector<int> data{};

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> label >> number;
      if (label == "Uid:") {
        result = number;
      }
    }
  }
  return (result);
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int uid) {
  string result{}, pwd{}, line{}, label{};
  int user_id{};

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      for (char& separator : line) {
        if (separator == ':') {
          separator = ' ';  // Replace ':' with ' '
        }
      }

      std::istringstream linestream(line);
      linestream >> label >> pwd >> user_id;

      if (uid == user_id) {
        result = label;
      }
    }
  }

  return (result);
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long ticks_per_second = sysconf(_SC_CLK_TCK);

  if (ticks_per_second == -1) {
    std::cerr << "Error retrieving clock ticks per second: " << errno
              << std::endl;
    return 1;
  }

  long process_uptime{};
  float jiffies{};
  string buffer{}, line{};
  std::vector<string> data{};
  long system_uptime = LinuxParser::UpTime();  // Get the system uptime

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);

      for (int i = 0; i < 22; i++) {
        linestream >> buffer;
      }
      jiffies = std::stof(buffer);
      process_uptime =
          system_uptime -
          static_cast<long>((jiffies / static_cast<float>(ticks_per_second)));
    }
  }
  return (process_uptime);
}
