#include <iostream>

#include "error.h"

void error_print(const int rc) {
    if (rc == 0)
        return;

    std::cout << "Ошибка" << std::endl;

    if (rc & FILE_DOESNT_EXIST)
        std::cout << "Выбранный файл не существует" << std::endl;
    if (rc & FILE_OPEN_ERROR)
        std::cout << "Не удалось открыть файл" << std::endl;
    if (rc & FILE_READ_ERROR)
        std::cout << "Не удалось прочитать файл" << std::endl;
    if (rc & STREAM_EMPTY_INPUT)
        std::cout << "Поток ввода пуст" << std::endl;
}
