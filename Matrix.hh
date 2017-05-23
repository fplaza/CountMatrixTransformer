#ifndef MATRIX_HH
#define MATRIX_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include <string>
#include <vector>
#include <cstring>

template <typename T>
class Matrix
{
    public:
        Matrix(const size_t nrow, const size_t ncol, bool init = true);
        Matrix(const Matrix<T>& other);
        ~Matrix();
        size_t nrow() const;
        size_t ncol() const;
        const std::vector<std::string>& rownames() const;
        const std::vector<std::string>& colnames() const;
        std::vector<std::string>& rownames();
        std::vector<std::string>& colnames();
        T operator()(const size_t i, const size_t j) const;
        T& operator()(const size_t i, const size_t j);
        void reset();

    private:
        size_t offset(const size_t i, const size_t j) const;
        const size_t nrow_;
        const size_t ncol_;
        std::vector<std::string> rownames_;
        std::vector<std::string> colnames_;
        T* data_;
};

template <typename T>
inline Matrix<T>::Matrix(const size_t nrow, const size_t ncol, bool init)
    : nrow_(nrow),
    ncol_(ncol),
    rownames_(nrow_),
    colnames_(ncol_),
    data_(new T[nrow_*ncol_])
{
    if (init)
    {
        reset();
    }
};

template <typename T>
inline Matrix<T>::Matrix(const Matrix<T>& other)
    : nrow_(other.nrow_),
    ncol_(other.ncol_),
    rownames_(other.rownames_),
    colnames_(other.colnames_),
    data_(new T[nrow_*ncol_])
{
    std::memcpy(data_, other.data_, nrow_*ncol_*sizeof(T));
}

template <typename T>
inline Matrix<T>::~Matrix()
{
    delete[] data_;
    data_ = NULL;
}

template <typename T>
inline size_t Matrix<T>::nrow() const
{
    return nrow_;
}

template <typename T>
inline size_t Matrix<T>::ncol() const
{
    return ncol_;
}

template <typename T>
const std::vector<std::string>& Matrix<T>::rownames() const
{
    return rownames_;
}

template <typename T>
const std::vector<std::string>& Matrix<T>::colnames() const
{
    return colnames_;
}

template <typename T>
std::vector<std::string>& Matrix<T>::rownames()
{
    return rownames_;
}

template <typename T>
std::vector<std::string>& Matrix<T>::colnames()
{
    return colnames_;
}


template <typename T>
T Matrix<T>::operator()(const size_t i, const size_t j) const
{
    return data_[offset(i,j)];
}

template <typename T>
T& Matrix<T>::operator()(const size_t i, const size_t j)
{
    return data_[offset(i,j)];
}

template <typename T>
void Matrix<T>::reset()
{
    std::memset(data_, 0, nrow_*ncol_*sizeof(T)); 
}

template <typename T>
size_t Matrix<T>::offset(const size_t i, const size_t j) const
{
    return i*ncol_ + j;
}

#endif // MATRIX_HH
