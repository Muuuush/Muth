#ifndef MUTH_Vec3_H
#define MUTH_Vec3_H

#include <string>
#include <sstream>

namespace Muth
{
    template<typename T>
    struct Vec3
    {   
    public:
        T x;
        T y;
        T z;

    public:
        inline Vec3() : x(0), y(0), z(0) {}
        inline Vec3(const T &x, const T &y, const T &z) : x(x), y(y), z(z) {}
        inline Vec3(const T *values) : x(values[0]), y(values[1]), z(values[2]) {}
    
    public:
        T& operator[] (const size_t &idx) { return idx == 0 ? x : (idx == 1 ? y : z); }

        std::string to_string(const std::string &separator = " ") const
        {
            std::stringstream ss;
            ss << x << separator << y << separator << z;
            return std::move(ss.str());
        }

    public:
        inline T length_square() const { return x * x + y * y + z * z; }
        inline T length() const
        {
            if (std::is_same<T, float>::value)
                return sqrtf(this->length_square());
            return sqrt(this->length_square());
        }
        inline Vec3<T> normalized() const { return (*this) / this->length(); }
        inline T projection(const Vec3<T> &vec) const { return this->dot(vec) / vec.length(); }
        inline  Vec3<T> projection_vector(const Vec3<T> &vec) const { return vec.normalized() * this->projection(vec); }
        inline T dot(const Vec3<T> &other) const { return x * other.x + y * other.y + z * other.z; }
    
    public:
        inline Vec3<T> &operator+= (const Vec3<T> &other) { *this = (*this) + other; return *this; }
        inline Vec3<T> &operator-= (const Vec3<T> &other) { *this = (*this) - other; return *this; }
        inline Vec3<T> &operator*= (T lambda) { *this = (*this) * lambda; return *this; }
        inline Vec3<T> &operator/= (T lambda) { *this = (*this) / lambda; return *this; }
    };

    template <typename T>
    inline Vec3<T> operator+(const Vec3<T> &left, const Vec3<T> &right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z };
    }

    template <typename T>
    inline Vec3<T> operator-(const Vec3<T> &left, const Vec3<T> &right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z };
    }

    template <typename T>
    inline Vec3<T> operator*(const Vec3<T> &vec, T lambda)
    {
        return { vec.x * lambda, vec.y * lambda, vec.z * lambda };
    }

    template <typename T>
    inline Vec3<T> operator/(const Vec3<T> &vec, T lambda)
    {
        return { vec.x / lambda, vec.y / lambda, vec.z / lambda };
    }

    template <typename T>
    inline Vec3<T> operator*(T lambda, const Vec3<T> &vec)
    {
        return { vec.x * lambda, vec.y * lambda, vec.z * lambda };
    }

    template <typename T>
    inline T operator*(const Vec3<T> &left, const Vec3<T> &right)
    {
        return left.dot(right);
    }

    template <typename T>
    inline Vec3<T> operator-(const Vec3<T> &vec)
    {
        return { -vec.x, -vec.y, -vec.z };
    }

    template <typename T>
    inline std::ostream &operator<<(std::ostream &out, const Vec3<T> &vec)
    {
        return out << vec.to_string();
    }

    template<typename T>
    inline Vec3<T> cross(Vec3<T> left, Vec3<T> right)
    {
        return {
            left[1] * right[2] - left[2] * right[1],
            left[2] * right[0] - left[0] * right[2],
            left[0] * right[1] - left[1] * right[0],
        };
    }
} // namespace Muth

#endif