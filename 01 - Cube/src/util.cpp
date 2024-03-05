#include "util.h"

long long calculateElapsedTimeInMilliseconds(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
