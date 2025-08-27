#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  string os, kernel, version;
  string line;
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
  vector<int> pids;
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

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> label >> number >> unit;
    memory.push_back(number);
  }

  mem_total = memory.at(0);
  mem_free = memory.at(1);
  mem_utilized = mem_total - mem_free;

  return static_cast<float>(mem_utilized) / static_cast<float>(mem_total);
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
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int num_processes{}, number{};
  string line{}, label{};
  std::vector<int> data{};

  std::ifstream stream(kProcDirectory + kStatFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> label >> number;
    data.push_back(number);
  }
  num_processes = data.at(8);
  return num_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int num_running_processes{}, number{};
  string line{}, label{}, unit{};
  std::vector<uint> data{};

  std::ifstream stream(kProcDirectory + kStatFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> label >> number >> unit;
    data.push_back(number);
  }

  num_running_processes = data.at(9);
  return (num_running_processes);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid [[maybe_unused]]) {
  string result{}, number{}, line{}, label{};
  std::vector<string> data{};

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> label >> number;
    data.push_back(number);
  }

  result = data.at(9);
  return (result);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int uid) {
  string result{}, pwd{}, line{}, label{};
  int user_id{};

  std::ifstream stream(kPasswordPath);

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

  return (result);
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
