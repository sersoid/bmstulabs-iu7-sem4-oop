#ifndef ERROR_H
#define ERROR_H

#include <map>
#include <string>
#include <vector>

#define OK 0

// File

#define FILE_DOESNT_EXIST 0b1
#define FILE_OPEN_ERROR 0b10
#define FILE_FORMAT_ERROR 0b100

// Render

#define EDGE_OUT_OF_VIEW 0b1000

std::string getErrorMessage(int rc);
#ifndef NDEBUG
void printErrorDebug(const std::map<std::string, std::vector<int>>& rc);
#endif

#endif
