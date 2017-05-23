#ifndef GENES_LENGTH_READER_HH
#define GENES_LENGTH_READER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Matrix.hh"
#include <vector>
#include <string>

class GenesLengthReader
{
    public:
        static std::vector<size_t> read(const std::string& genes_length_file, Matrix<float>& cm);
    private:
        static const size_t MAX_LINE_LEN;
};

#endif //GENES_LENGTH_READER_HH

