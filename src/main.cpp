#include "ncurses_display.h"
#include "system.h"
#include "process.h"
#include <iostream>
#include <vector>

using std::vector;
int main() {
  System system;
  NCursesDisplay::Display(system);

}