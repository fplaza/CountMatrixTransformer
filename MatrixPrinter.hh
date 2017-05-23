#ifndef MATRIX_PRINTER_HH
#define MATRIX_PRINTER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Matrix.hh"
#include <iostream>
#include <fstream>
#include <string>

template <typename T>
class MatrixPrinter
{
    public:
        static void print(const Matrix<T>& matrix, const std::string& output_file);
};

template <typename T>
void MatrixPrinter<T>::print(const Matrix<T>& matrix, const std::string& output_file)
{
    std::cout << "Progress: 0%" << std::flush;
    clock_t next_time = clock()+2*CLOCKS_PER_SEC;

    std::ofstream ofs;
    ofs.open(output_file.c_str(), std::ios_base::app);

    ofs << "gene_name";
    for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
    {
        ofs << '\t' << matrix.colnames()[curr_col];
    }

    ofs << '\n';

    for (size_t curr_row = 0; curr_row < matrix.nrow(); ++curr_row)
    {
        ofs << matrix.rownames()[curr_row];

        for (size_t curr_col = 0; curr_col < matrix.ncol(); ++curr_col)
        {
            const T val = matrix(curr_row, curr_col);

            if (val == T(0))
            {
                ofs << "\t0";
            }
            else
            {
                ofs << '\t' << val;
            }
        }

        ofs << '\n';

        if (clock() > next_time)
        {
            const unsigned int perc_progress =
                static_cast<unsigned int>(100.0*curr_row/matrix.nrow());

            std::cout << "\rProgress: " << perc_progress << '%' << std::flush;

            next_time = clock()+2*CLOCKS_PER_SEC;
        }

    }

    ofs.close();
}


#endif // MATRIX_PRINTER_HH
