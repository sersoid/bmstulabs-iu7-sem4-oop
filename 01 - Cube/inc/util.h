#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <utility>

long long calculateElapsedTimeInMilliseconds(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);

template<typename function, typename functionOut, typename... functionArgs>
long long doWithElapsedTime(function&& func, functionOut&& out, functionArgs&&... args) {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    if constexpr (std::is_same_v<functionOut, void*> || std::is_void_v<std::invoke_result_t<function, functionArgs...>>)
        std::forward<function>(func)(std::forward<functionArgs>(args)...);
    else
        out = std::forward<function>(func)(std::forward<functionArgs>(args)...);

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return calculateElapsedTimeInMilliseconds(start, end);
}

#endif
