#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <utility>

long long calculateElapsedTimeInMilliseconds(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

template<typename function, typename... functionArgs>
long long doWithElapsedTime(function&& func, functionArgs&&... args) {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::forward<function>(func)(std::forward<functionArgs>(args)...);
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return calculateElapsedTimeInMilliseconds(start, end);
}

#endif
