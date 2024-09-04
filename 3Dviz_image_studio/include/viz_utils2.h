#ifndef viz_utils2_H
#define viz_utils2_H

#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

std::string new_directory();
std::string new_directory2();
std::string new_directory3();
bool copyFile(const char *SRC, const char* DEST);

#endif
