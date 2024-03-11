#include <string>

#include "error.h"

std::string printError(const int rc) {
    std::string errorString;

    if (rc & FILE_DOESNT_EXIST)
        errorString += "Такого файла не существует\n";
    if (rc & FILE_OPEN_ERROR)
        errorString += "Не удалось открыть файл\n";
    if (rc & FILE_FORMAT_ERROR)
        errorString += "Файл имеет некорректный формат\n";

    return errorString;
}
