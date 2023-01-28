#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <linux_parser.h>
#include <unistd.h>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    long total_time;
    long uptime;
    float cpu_usage;
    total_time = LinuxParser::ActiveJiffies(pid_);
    uptime = LinuxParser::UpTime(pid_);
    cpu_usage = ((float)total_time/(float)sysconf(_SC_CLK_TCK))/(float)uptime;
    return cpu_usage; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string ramkb = LinuxParser::Ram(pid_);
    long rammb = std::stol(ramkb)/1000;
    return std::to_string(rammb); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->CpuUtilization()<a.CpuUtilization();
    }