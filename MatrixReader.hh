#ifndef MATRIX_READER_HH
#define MATRIX_READER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Matrix.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>

template <typename T>
class MatrixReader
{
    public:
        static Matrix<T> read(const std::string& matrix_file);

    private:
        static const size_t MAX_LINE_LENGTH;
        struct ConversionFunction;

        const std::string matrix_file_;
};

namespace
{
    bool is_separator(const char c)
    {
        return c == '\t' || c == ' ';
    }
}

template <typename T>
Matrix<T> MatrixReader<T>::read(const std::string& matrix_file)
{
    std::cout << "Progress: 0%" << std::flush;
    clock_t next_time = clock()+2*CLOCKS_PER_SEC;

    char line[MAX_LINE_LENGTH];

    std::ifstream ifs;
    ifs.open(matrix_file.c_str());
    ifs.seekg(0,std::ios_base::end);
    const std::streampos end_pos = ifs.tellg();
    ifs.seekg(0,std::ios_base::beg);

    // FIRST PASS: get the dimensions of the matrix
    // Get the number of columns
    std::string header;
    std::getline(ifs, header);
    size_t ncol = std::count_if(header.begin(), header.end(), is_separator);

    // Get the number of rows
    size_t nrow = 0;
    while (ifs.getline(line, MAX_LINE_LENGTH))
        ++nrow;

    // Allocate memory to store the count matrix
    Matrix<T> matrix(nrow, ncol, false);

    //SECOND PASS: fill the matrix
    ifs.clear();
    ifs.seekg(0);

    // Skip potential first column string
    while (!std::isspace(ifs.get()))
        ;

    // Read colnames
    for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
    {
        ifs >> matrix.colnames()[curr_col];
    }

    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Read rownames and matrix content
    for (size_t curr_row = 0; curr_row < matrix.nrow(); ++curr_row)
    {
        ifs.getline(line, MAX_LINE_LENGTH);
        char* pos = line;

        while (*pos != '\0' && !std::isspace(*pos))
            ++pos;

        const std::string rowname(line, pos-line);
        matrix.rownames()[curr_row] = rowname;

        ConversionFunction conversion_function;
        for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
        {
            matrix(curr_row, curr_col) = conversion_function(pos, &pos);
        }

        if (clock() > next_time)
        {
            const std::streampos curr_pos = ifs.tellg();

            const unsigned int perc_progress =
                static_cast<unsigned int>(100.0*curr_pos/end_pos);

            std::cout << "\rProgress: " << perc_progress << '%' << std::flush;

            next_time = clock()+2*CLOCKS_PER_SEC;
        }
    }

    ifs.close();

    return matrix;
}

template <>
struct MatrixReader<unsigned long>::ConversionFunction
{
    unsigned long operator()(const char *nptr, char **endptr)
    {
        return strtoul(nptr, endptr, 10);
    }
};

template <>
struct MatrixReader<float>::ConversionFunction
{
    float operator()(const char *nptr, char **endptr)
    {
        return strtof(nptr, endptr);
    }
};

template <>
struct MatrixReader<double>::ConversionFunction
{
    double operator()(const char *nptr, char **endptr)
    {
        return strtod(nptr, endptr);
    }
};

template <typename T>
const size_t MatrixReader<T>::MAX_LINE_LENGTH = 100000;

#endif // MATRIX_READER_HH
