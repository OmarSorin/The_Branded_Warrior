#ifndef MATHUTILS_H
#define MATHUTILS_H

namespace util {

    // cppcheck-suppress unusedFunction
    template <typename T>
    [[nodiscard]] T clamp(T value, T lo, T hi) {
        if (value < lo) return lo;
        if (value > hi) return hi;
        return value;
    }

}

#endif // MATHUTILS_H