#ifndef MUTH_VECTOR_H
#define MUTH_VECTOR_H

#include <string>

namespace Muth
{
    template <typename T, size_t n>
    struct Vector
    {
        T *elements;

    public:
        Vector();
        Vector(const Vector<T, n> &src);
        void operator= (const Vector<T, n> &src);
        Vector(const T *values);
        Vector(const std::initializer_list<T> &values);
        Vector(Vector<T, n> &&r_value);
        ~Vector();
    
    public:
        T& operator[] (const size_t &idx) const;
        inline T &get_ref(const size_t &idx) const;

        std::string to_string(const std::string &separator = " ") const;
        operator std::string() const;

    public:
        T length_square() const;
        template<typename R>
        R length() const;
        Vector<T, n> dot(Vector<T, n> other) const;
    };

    template <typename T, size_t n>
    inline Vector<T, n>::Vector()
    {
        elements = new T[n];
    }

    template <typename T, size_t n>
    inline Vector<T, n>::Vector(const Vector<T, n> &src)
    {
        elements = new T[n]{};
        memcpy(elements, src.elements, n * sizeof(T));
    }

    template <typename T, size_t n>
    inline void Vector<T, n>::operator=(const Vector<T, n> &src)
    {
        memcpy(elements, src.elements, n * sizeof(T));
    }

    template <typename T, size_t n>
    inline Vector<T, n>::Vector(const T *values)
    {
        elements = new T[n]{};
        memcpy(elements, values, n * sizeof(T));
    }

    template <typename T, size_t n>
    inline Vector<T, n>::Vector(const std::initializer_list<T> &values)
    {
        elements = new T[n]{};
        std::copy(values.begin(), values.end(), elements);
    }

    template <typename T, size_t n>
    inline Vector<T, n>::Vector(Vector<T, n> &&r_value)
    {
        elements = r_value.elements;
        r_value.elements = nullptr;
    }

    template <typename T, size_t n>
    inline Vector<T, n>::~Vector()
    {
        delete[] elements;
    }

    template <typename T, size_t n>
    inline T& Vector<T, n>::operator[](const size_t &idx) const
    {
        return elements[idx];
    }

    template <typename T, size_t n>
    inline T &Vector<T, n>::get_ref(const size_t &idx) const
    {
        if (0 <= idx && idx < n)
            return elements[idx];
        else
            throw MuthOutOfRangeException("vector index out of range");
    }

    template <typename T, size_t n>   
    inline std::string Vector<T, n>::to_string(const std::string &separator /*= " "*/) const
    {
        std::stringstream ss;
        for (size_t i = 0; i < n; i++)
            ss << this->elements[i] << separator;
        return std::move(ss.str());
    }

    template <typename T, size_t n>
    inline T Vector<T, n>::length_square() const
    {
        T result = (T)0;
        for (size_t i = 0; i < n; i++)
            result += elements[i] * elements[i];
        return result;
    }

    template <typename T, size_t n>
    template <typename R>
    inline R Vector<T, n>::length() const
    {
        return (R)sqrt((double)this->length_square());
    }

    template <typename T, size_t n>
    Vector<T, n> operator+(const Vector<T, n> &left, const Vector<T, n> &right)
    {
        Vector<T, n> result;
        for (size_t i = 0; i < n; i++)
            result.elements[i] = left[i] + right[i];
        return std::move(result);
    }

    template <typename T, size_t n>
    Vector<T, n> operator*(const Vector<T, n> &vec, T lambda)
    {
        Vector<T, n> result;
        for (size_t i = 0; i < n; i++)
            result.elements[i] = vec[i] * lambda;
        return std::move(result);
    }

    template <typename T, size_t n>
    Vector<T, n> operator*(T lambda, const Vector<T, n> &vec)
    {
        Vector<T, n> result;
        for (size_t i = 0; i < n; i++)
            result.elements[i] = vec[i] * lambda;
        return std::move(result);
    }

    template <typename T, size_t n>
    inline Vector<T, n> Vector<T, n>::dot(Vector<T, n> other) const
    {
        Vector<T, n> result;
        for (size_t i = 0; i < n; i++)
            result.elements[i] = elements[i] * other[i];
        return std::move(result);
    }

    template <typename T, size_t n>
    Vector<T, n> operator*(const Vector<T, n> &left, const Vector<T, n> &right)
    {
        return std::move(left.dot(right));
    }

    template <typename T, size_t n>
    Vector<T, n> operator-(const Vector<T, n> &vec)
    {
        Vector<T, n> result;
        for (size_t i = 0; i < n; i++)
            result.elements[i] = -vec[i];
        return std::move(result);
    }

    template <typename T, size_t n>
    std::ostream &operator<<(std::ostream &out, const Vector<T, n> &vec)
    {
        return out << vec.to_string();
    }

    template<typename T>
    using Vec2 = Vector<T, 2ULL>;

    template<typename T>
    using Vec3 = Vector<T, 3ULL>;

    template<typename T>
    T cross(Vec2<T> left, Vec2<T> right)
    {
        return left[0] * right[1] - left[1] * right[0];
    }

    template<typename T>
    Vec3<T> cross(Vec3<T> left, Vec3<T> right)
    {
        return {
            left[1] * right[2] - left[2] * right[1],
            left[2] * right[0] - left[0] * right[2],
            left[0] * right[1] - left[1] * right[0],
        };
    }
} // namespace Muth

#endif