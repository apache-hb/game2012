#pragma once

#include "math.h"

template<typename T>
struct std::hash<math::Vec2<T>> {
    size_t operator()(math::Vec2<T> const &v) const {
        return std::hash<T>()(v.x)
             ^ std::hash<T>()(v.y);
    }
};

template<typename T>
struct std::hash<math::Vec3<T>> {
    size_t operator()(math::Vec3<T> const &v) const {
        return std::hash<T>()(v.x)
             ^ std::hash<T>()(v.y)
             ^ std::hash<T>()(v.z);
    }
};

template<typename T>
struct std::hash<math::Vec4<T>> {
    size_t operator()(math::Vec4<T> const &v) const {
        return std::hash<T>()(v.x)
             ^ std::hash<T>()(v.y)
             ^ std::hash<T>()(v.z)
             ^ std::hash<T>()(v.w);
    }
};
