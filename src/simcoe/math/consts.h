#pragma once

namespace math {
    template<typename T>
    constexpr T kPi = T(3.14159265358979323846264338327950288);

    template<typename T>
    constexpr T kRadToDeg = T(180) / kPi<T>;

    template<typename T>
    constexpr T kDegToRad = kPi<T> / T(180);
}
