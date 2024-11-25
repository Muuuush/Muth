#ifndef MUTH_NUM_TOOL_H
#define MUTH_NUM_TOOL_H

#include <limits>
#include <cmath>

namespace Muth
{

    template <typename T>
    inline bool nonzero(T obj) { return false; }
    template <>
    inline bool nonzero(short obj) { return obj != 0; }
    template <>
    inline bool nonzero(int obj) { return obj != 0; }
    template <>
    inline bool nonzero(long obj) { return obj != 0L; }
    template <>
    inline bool nonzero(long long obj) { return obj != 0LL; }
    template <>
    inline bool nonzero(float obj) { return std::abs(obj) >= std::numeric_limits<float>::epsilon(); }
    template <>
    inline bool nonzero(double obj) { return std::abs(obj) >= std::numeric_limits<double>::epsilon(); }

    template <typename T>
    inline bool equal(T x, T y) { return x == y; }
    template <>
    inline bool equal(float x, float y) { return std::abs(x - y) < std::numeric_limits<float>::epsilon(); }
    template <>
    inline bool equal(double x, double y) { return std::abs(x - y) < std::numeric_limits<float>::epsilon(); }

} // namespace Muth

#endif