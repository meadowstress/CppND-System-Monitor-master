#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  Process(int pid)
      : pid_{pid},
        uid_{},
        cpu_utilization_{},
        command_{},
        ram_{},
        up_time_{},
        user_{} {};

  // TODO: Declare any necessary private members
 private:
  int pid_;
  int uid_;
  float cpu_utilization_;
  std::string command_;
  std::string user_;
  std::string ram_;
  long int up_time_;
};

#endif