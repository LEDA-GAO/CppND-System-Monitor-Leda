#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

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
  string line;
  string key;
  string MemTotal;
  string MemFree;
  float mem_total; 
  float mem_free;
  float mem_uti;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open()){
        std::getline(stream, line);
        std::istringstream linestream1(line);
        linestream1>>key>>MemTotal;
        std::getline(stream, line);
        std::istringstream linestream2(line);
        linestream2>>key>>MemFree;        
  }
  mem_total = std::stof(MemTotal);
  mem_free = std::stof(MemFree);
  mem_uti = (mem_total-mem_free)/mem_total;
  return mem_uti; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime;
  long up_time;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  up_time = std::stof(uptime);
  return up_time; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string key;
  string utime;
  string stime;
  string cstime;
  string cutime;
  int ncount = 1;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line); 
    while(linestream>>key){
      if(ncount==13) break;
      ncount+=1;
    }
    linestream>>utime>>stime>>cstime>>cutime;
  }
  return std::stol(utime)+std::stol(stime)+std::stol(cstime)+std::stol(cutime); }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpudata =  LinuxParser::CpuUtilization();
  long NonIdle;
  NonIdle = std::stol(cpudata[0])+std::stol(cpudata[1])+std::stol(cpudata[2])+std::stol(cpudata[5])
  +std::stol(cpudata[6])+std::stol(cpudata[7]);
  return NonIdle; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpudata =  LinuxParser::CpuUtilization();
  long Idle;
  Idle = std::stol(cpudata[3])+std::stol(cpudata[4]);
  return Idle;
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string CPU;
  vector<string> cpudata(10);
  std::ifstream stream(kProcDirectory+kStatFilename);
    if (stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream>>CPU;
      for(int i = 0; i<10; i++){
      linestream>>cpudata[i];
      } 
    }
     
  return cpudata; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string total_process;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream>>key>>total_process)
      {
        if(key=="processes") return stoi(total_process);
      }
    }
  }
  return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line;
  string key;
  string running_process;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream>>key>>running_process)
      {
        if(key=="procs_running") return stoi(running_process);
      }
    }
  }
  return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
  }
  return line; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string ram;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (stream.is_open()){
        while(std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream>>key>>ram;
        if(key=="VmSize:") return ram;
        }      
  }
 return ram;}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string uid;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      while (linestream>>key>>uid)
      {
        if(key=="Uid:") return uid;
      }
    }
  }
  return uid; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string username;
  string x;
  string uid;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream>>username>>x>>uid){
        if(uid == LinuxParser::Uid(pid)) return username;
      }
    }
  }
  return username; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string key;
  string starttime;
  long uptime;
  int ncount = 1;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line); 
    while(linestream>>key){
      if(ncount==21) break;
      ncount+=1;
    }
    linestream>>starttime;
  }
  uptime = LinuxParser::UpTime()-std::stol(starttime)/sysconf(_SC_CLK_TCK);
  return uptime; }
