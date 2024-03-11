#include "error.h"

std::string printError(const int rc) {
    std::string errorString;

    // File

    if (rc & FILE_DOESNT_EXIST)
        errorString += "Такого файла не существует.\n";
    if (rc & FILE_OPEN_ERROR)
        errorString += "Не удалось открыть файл.\n";
    if (rc & FILE_FORMAT_ERROR)
        errorString += "Файл имеет некорректный формат.\n";

    // Render

    if (rc & EDGE_OUT_OF_VIEW)
        errorString += "Некоторые ребра вне экрана, их отрисовка пропущена.\n";

    return errorString;
}
