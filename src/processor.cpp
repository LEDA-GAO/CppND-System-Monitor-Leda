#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>
#include <vector>
#include<thread>
#include<chrono>

using std::string;
using std::vector;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  //long Idle, PrevIdle, Total, PrevTotal, totald, idled;
  //long NonIdle, PrevNonIdle;
  long Idle, NonIdle, Total;
  float CPU_Percentage;
  //PrevIdle = LinuxParser::IdleJiffies();
  //PrevNonIdle = LinuxParser::ActiveJiffies();
  
  //std::this_thread::sleep_for(std::chrono::seconds(1));

  Idle = LinuxParser::IdleJiffies();

  NonIdle = LinuxParser::ActiveJiffies();


  //PrevTotal = PrevIdle+PrevNonIdle;
  Total = Idle+NonIdle;

  //totald = Total-PrevTotal;
  //idled = Idle - PrevIdle;
  //CPU_Percentage = (float)(totald-idled)/(float)totald;
  CPU_Percentage = (float)NonIdle/(float)Total;
  return CPU_Percentage; }