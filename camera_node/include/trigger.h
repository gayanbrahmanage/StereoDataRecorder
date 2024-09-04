#ifndef trigger_H
#define trigger_H
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>

#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

class trigger{
public:
  int pulse=7;
  int serial_port;



  trigger();
  ~trigger();
  int init();
  void terminate();
  void send_trigger();
};

#endif
