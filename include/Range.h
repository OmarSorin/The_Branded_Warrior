#ifndef RANGE_H
#define RANGE_H

#include <random>
#include <type_traits>

namespace util {

    template <typename T>
    class Range {
        static_assert(std::is_arithmetic_v<T>,
                      "Range requires an arithmetic type");

        T minVal;
        T maxVal;

    public:
        constexpr Range(T minValue, T maxValue)
            : minVal(minValue), maxVal(maxValue) {}

        // cppcheck-suppress unusedFunction
        [[nodiscard]] T getRandom() const {
            static std::mt19937 rng{std::random_device{}()};
            if constexpr (std::is_integral_v<T>) {
                std::uniform_int_distribution<T> dist(minVal, maxVal);
                return dist(rng);
            } else {
                std::uniform_real_distribution<T> dist(minVal, maxVal);
                return dist(rng);
            }
        }
    };

}

#endif // RANGE_H