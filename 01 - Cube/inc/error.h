#ifndef __H_ERROR
#define __H_ERROR

#define OK 0
#define FILE_DOESNT_EXIST 0b1
#define FILE_OPEN_ERROR 0b01
#define FILE_READ_ERROR 0b001
#define STREAM_EMPTY_INPUT 0b0001

void error_print(int rc);

#endif
