#ifndef MUTH_MATRIX_H
#define MUTH_MATRIX_H

#include <string>
#include <sstream>

#include "NumTool.h"
#include "MuthException.h"
#include "Vector.h"

namespace Muth
{

    template <typename T, size_t n, size_t m>
    struct Matrix
    {
    public:
        T *elements;

    public:
        Matrix();
        Matrix(const Matrix<T, n, m> &src);
        Matrix(const T *values);
        Matrix(const std::initializer_list<T> &values);
        Matrix(Matrix<T, n, m> &&r_value);
        ~Matrix();

        Matrix<T, n, m> &operator=(Matrix<T, n, m> &&other);
        Matrix<T, n, m> &operator+=(Matrix<T, n, m> &&other);
        Matrix<T, n, m> &operator-=(Matrix<T, n, m> &&other);
        Matrix<T, n, m> &operator*=(T lambda);
        Matrix<T, n, m> &operator/=(T lambda);

        inline T *operator[](const size_t &row) const;
        inline T &get_ref(const size_t &row, const size_t &col) const;

        std::string to_string(const std::string &separator = " ", const std::string &end_row = "") const;
        operator std::string() const;

        operator T *() const;

        inline Matrix<T, m, n> transform() const;

        inline void row_multiply(const size_t &r, const T &k);
        inline void row_swap(const size_t &r1, const size_t &r2);
        inline void row_add_to(const size_t &added_r, const size_t &add_r, T k);

        void gaussian_eliminate();
        T det() const;

        bool reversible() const;
    };

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::Matrix()
    {
        elements = new T[n * m]{};
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::Matrix(const Matrix<T, n, m> &src)
    {
        elements = new T[n * m]{};
        memcpy(elements, src.elements, n * m * sizeof(T));
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::Matrix(const T *values)
    {
        elements = new T[n * m]{};
        memcpy(elements, values, n * m * sizeof(T));
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, n, m>::Matrix(const std::initializer_list<T> &values)
    {
        elements = new T[n * m]{};
        std::copy(values.begin(), values.end(), elements);
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::Matrix(Matrix<T, n, m> &&r_value)
    {
        elements = r_value.elements;
        r_value.elements = nullptr;
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::~Matrix()
    {
        delete[] elements;
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> &Matrix<T, n, m>::operator=(Matrix<T, n, m> &&other)
    {
        elements = other.elements;
        other.elements = nullptr;
        return *this;
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, n, m> &Matrix<T, n, m>::operator+=(Matrix<T, n, m> &&other)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                (*this)[r][c] += other[r][c];
        return *this;
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, n, m> &Matrix<T, n, m>::operator-=(Matrix<T, n, m> &&other)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                (*this)[r][c] -= other[r][c];
        return *this;
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, n, m> &Matrix<T, n, m>::operator*=(T lambda)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                (*this)[r][c] *= lambda;
        return *this;
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, n, m> &Matrix<T, n, m>::operator/=(T lambda)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                (*this)[r][c] /= lambda;
        return *this;
    }

    template <typename T, size_t n, size_t m>
    inline T *Matrix<T, n, m>::operator[](const size_t &row) const
    {
        return elements + (row * m);
    }

    template <typename T, size_t n, size_t m>
    inline T &Matrix<T, n, m>::get_ref(const size_t &row, const size_t &col) const
    {
        if (row < n && col < m)
            return elements[row * m + col];
        else
            throw MuthOutOfRangeException("matrix index out of range");
    }

    template <typename T, size_t n, size_t m>
    std::string Matrix<T, n, m>::to_string(const std::string &separator /*= " "*/, const std::string &end_row /*= ""*/) const
    {
        std::stringstream ss;
        for (size_t r = 0; r < n; r++)
        {
            for (size_t c = 0; c < m; c++)
            {
                ss << elements[r * m + c] << separator;
            }
            ss << end_row;
        }
        return std::move(ss.str());
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::operator std::string() const
    {
        return this->to_string();
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m>::operator T *() const
    {
        return elements;
    }

    template <typename T, size_t n, size_t m>
    inline Matrix<T, m, n> Matrix<T, n, m>::transform() const
    {
        Matrix<T, m, n> t;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                t[c][r] = elements[r][c];
        return std::move(t);
    }

    template <typename T, size_t n, size_t m>
    inline void Matrix<T, n, m>::row_multiply(const size_t &r, const T &k)
    {
        T *row = elements + r * m;
        for (size_t i = 0; i < m; i++)
            row[i] *= k;
    }

    template <typename T, size_t n, size_t m>
    inline void Matrix<T, n, m>::row_swap(const size_t &r1, const size_t &r2)
    {
        for (size_t i = 0; i < m; i++)
            std::swap(elements[r1 * m + i], elements[r2 * m + i]);
    }

    template <typename T, size_t n, size_t m>
    inline void Matrix<T, n, m>::row_add_to(const size_t &added_r, const size_t &add_r, T k)
    {
        for (size_t i = 0; i < m; i++)
            elements[added_r * m + i] += elements[add_r * m + i] * k;
    }

    template <typename T, size_t n, size_t m>
    void Matrix<T, n, m>::gaussian_eliminate()
    {
        for (int i = 0; i < m; i++)
        {
            int nonzero = i;
            for (nonzero = i; nonzero < n; nonzero++)
                if (Muth::nonzero(elements[nonzero * m + i]))
                    break;
            if (nonzero >= n)
            {
                i++;
                continue;
            }
            if (nonzero != i)
                row_swap(i, nonzero);
            for (size_t r = i + 1; r < n; r++)
            {
                row_add_to(r, i, -elements[r * m + i] / elements[i * m + i]);
            }
        }
    }

    template <typename T, size_t n, size_t m>
    T Matrix<T, n, m>::det() const
    {
        if (n != m)
        {
            throw MuthExceptionInvalidOperation("trying to calculate determinant of a non-square matrix");
        }
        else
        {
            Matrix<T, n, m> tmp(*this);
            tmp.gaussian_eliminate();
            T ans = tmp[0][0];
            for (size_t i = 1; i < n; i++)
                ans *= tmp[i][i];
            return ans;
        }
    }

    template <typename T, size_t n, size_t m>
    inline bool Matrix<T, n, m>::reversible() const
    {
        if (n != m)
            return false;

        Matrix<T, n, m> tmp(*this);
        tmp.gaussian_eliminate();
        return nonzero(tmp[n * m - 1]);
    }

    template <typename T, size_t n, size_t m>
    std::ostream &operator<<(std::ostream &out, const Matrix<T, n, m> &mat)
    {
        return out << mat.to_string();
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> &operator-(Matrix<T, n, m> &mat)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                mat[r][c] = -mat[r][c];
        return mat;
    }

    template <typename T, size_t n, size_t m, size_t w>
    Matrix<T, n, w> operator*(const Matrix<T, n, m> &left, const Matrix<T, m, w> &right)
    {
        Matrix<T, n, w> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < w; c++)
                for (size_t k = 0; k < m; k++)
                    result[r][c] += left[r][k] * right[k][c];
        return std::move(result);
    }
    
    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> operator*(const Matrix<T, n, m> &mat, T lambda)
    {
        Matrix<T, n, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r][c] = mat[r][c] * lambda;
        return std::move(result);
    }
    
    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> operator/(const Matrix<T, n, m> &mat, T lambda)
    {
        Matrix<T, n, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r][c] = mat[r][c] / lambda;
        return std::move(result);
    }
    
    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> operator*(T lambda, const Matrix<T, n, m> &mat)
    {
        Matrix<T, n, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r][c] = mat[r][c] * lambda;
        return std::move(result);
    }

    template <typename T, size_t n, size_t m>
    Vector<T, n> operator*(const Matrix<T, n, m> &mat, const Vector<T, m> &vec)
    {
        Vector<T, n> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r] += mat[r][c] * vec[c];
        return std::move(result);
    }

    template <typename T, size_t n, size_t m>
    Vector<T, m> operator*(const Vector<T, n> &vec, const Matrix<T, n, m> &mat)
    {
        Vector<T, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[c] += mat[r][c] * vec[r];
        return std::move(result);
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> operator+(const Matrix<T, n, m> &left, const Matrix<T, n, m> &right)
    {
        Matrix<T, n, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r][c] = left[r][c] + right[r][c];
        return std::move(result);
    }

    template <typename T, size_t n, size_t m>
    Matrix<T, n, m> operator-(const Matrix<T, n, m> &left, const Matrix<T, n, m> &right)
    {
        Matrix<T, n, m> result;
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < m; c++)
                result[r][c] = left[r][c] - right[r][c];
        return std::move(result);
    }
    
    template <typename T, size_t n, size_t m, size_t w>
    bool operator==(const Matrix<T, n, m> &left, const Matrix<T, m, w> &right)
    {
        for (size_t r = 0; r < n; r++)
            for (size_t c = 0; c < w; c++)
                if (!equal(left[r][c], right[r][c]))
                    return false;
        return true;
    }

    template <typename T>
    using Mat2 = Matrix<T, 2ULL, 2ULL>;
    template <typename T>
    using Mat3 = Matrix<T, 3ULL, 3ULL>;
    template <typename T>
    using Mat4 = Matrix<T, 4ULL, 4ULL>;

} // namespace Muth

#endif