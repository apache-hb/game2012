#pragma once

#include "consts.h"

#include <compare>

// rotation units
namespace math {
    template<typename T> struct Degrees;
    template<typename T> struct Radians;

    // a value stored in degrees
    template<typename T>
    struct Degrees {
        constexpr Degrees(T value = T(0))
            : value(value)
        { }

        constexpr Degrees(Radians<T> rad)
            : value(rad.degrees())
        { }

        constexpr T radians() const { return value * kDegToRad<T>; }
        constexpr T degrees() const { return value; }

        constexpr Degrees operator-() const { return Degrees(-value); }

        constexpr Degrees& operator+=(Degrees rhs) {
            value += rhs.value;
            return *this;
        }

        constexpr Degrees& operator-=(Degrees rhs) {
            value -= rhs.value;
            return *this;
        }

        constexpr std::partial_ordering operator<=>(Degrees rhs) const {
            return value <=> rhs.value;
        }

    private:
        T value;
    };

    // a value stored in radians
    template<typename T>
    struct Radians {
        constexpr Radians(T value = T(0))
            : value(value)
        { }

        constexpr Radians(Degrees<T> deg)
            : value(deg.radians())
        { }

        constexpr T radians() const { return value; }
        constexpr T degrees() const { return value * kRadToDeg<T>; }

        constexpr Radians operator-() const { return Radians(-value); }

    private:
        T value;
    };

    // scalar ops
    template<typename T>
    constexpr T radians(T it) { return it; }

    template<typename T>
    constexpr T radians(Degrees<T> deg) { return deg.radians(); }

    template<typename T>
    constexpr T radians(Radians<T> rad) { return rad.radians(); }

    template<typename T>
    constexpr T degrees(T it) { return it; }

    template<typename T>
    constexpr T degrees(Degrees<T> deg) { return deg.degrees(); }

    template<typename T>
    constexpr T degrees(Radians<T> rad) { return rad.degrees(); }

    // vector ops
    template<template<typename> typename U, typename T>
    constexpr U<Radians<T>> radians(U<Radians<T>> rad) { return rad; }

    template<template<typename> typename U, typename T>
    constexpr U<Radians<T>> radians(U<Degrees<T>> rad) {
        return rad * kDegToRad<T>;
    }

    template<template<typename> typename U, typename T>
    constexpr U<Degrees<T>> degrees(U<Degrees<T>> deg) { return deg; }

    template<template<typename> typename U, typename T>
    constexpr U<Degrees<T>> degrees(U<Radians<T>> rad) {
        return rad * kRadToDeg<T>;
    }
}
