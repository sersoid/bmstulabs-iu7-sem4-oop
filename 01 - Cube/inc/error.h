#ifndef ERROR_H
#define ERROR_H

#define OK 0
#define FILE_DOESNT_EXIST 0b1
#define FILE_OPEN_ERROR 0b10
#define FILE_FORMAT_ERROR 0b100

std::string printError(int rc);

#endif
