#include "error.h"

#include <QDebug>

std::string getErrorMessage(const int rc) {
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

#ifndef NDEBUG
void printErrorDebug(const std::map<std::string, std::vector<int>>& rc) {
    for (const std::pair<std::string, std::vector<int>> keyValue : rc) {
        std::string errorTrace = "Ошибки [" + keyValue.first + "]:\n";
        bool isErrorTrace = false;

        for (size_t i = 0; i < keyValue.second.size(); i++) {
            std::string errorMessage = getErrorMessage(keyValue.second[i]);

            if (! errorMessage.empty()) {
                errorTrace += keyValue.first + "#" + std::to_string(i) + ": " + errorMessage;
                isErrorTrace = true;
            }
        }

        if (isErrorTrace)
            qDebug() << QString::fromStdString(errorTrace);
    }
}
#endif
