#ifndef COUNT_MATRIX_TRANSFORMER_HH
#define COUNT_MATRIX_TRANSFORMER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Matrix.hh"
#include <memory>

class CountMatrixTransformer
{
    public:
        virtual void transform(Matrix<float>& count_matrix, const std::vector<size_t>& genes_length) = 0;
        virtual ~CountMatrixTransformer(){};
};

class CountMatrixNormalizer : public CountMatrixTransformer
{
    public:
        virtual void transform(Matrix<float>& count_matrix, const std::vector<size_t>& genes_length);
};

class CountMatrixDenormalizer : public CountMatrixTransformer
{
    public:
        CountMatrixDenormalizer(const double min_non_null);
        virtual void transform(Matrix<float>& count_matrix, const std::vector<size_t>& genes_length);

    private:
        const double min_non_null_;
};

inline CountMatrixDenormalizer::CountMatrixDenormalizer(const double min_non_null)
    : min_non_null_(min_non_null)
{}

class CountMatrixTransformerFactory
{
    public:
        static std::auto_ptr<CountMatrixTransformer> create_transformer(const std::string& transformation_type, const double min_non_null);
    private:
        static const char NORMALIZE_TRANSFORMATION_TYPE[];
        static const char DENORMALIZE_TRANSFORMATION_TYPE[];
};

#endif //COUNT_MATRIX_TRANSFORMER_HH
