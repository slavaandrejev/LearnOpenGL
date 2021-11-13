#pragma once

#include <cstdlib>
#include <ctime>

inline double operator - (const timespec &t1, const timespec &t2) {
    auto s  = int64_t{t1.tv_sec  - t2.tv_sec};
    auto ns = int64_t{t1.tv_nsec - t2.tv_nsec};
    if (0 <= s) {
        if (0 > ns) {
            --s;
            ns += 1'000'000'000;
        }
    } else {
        if (0 < ns) {
            ++s;
            ns -= 1'000'000'000;
        }
    }
    return s + ns * 1e-9;
}
