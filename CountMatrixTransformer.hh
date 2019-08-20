#ifndef COUNT_MATRIX_TRANSFORMER_HH
#define COUNT_MATRIX_TRANSFORMER_HH

/*
 * Copyright 2016-2017 Enterome
 */ 

#include "Matrix.hh"
#include "GeneMetadata.hh"
#include <memory>

class CountMatrixTransformer
{
    public:
        virtual void transform(Matrix<float>& count_matrix, const std::vector<GeneMetadata>& genes_metadata) = 0;
        virtual ~CountMatrixTransformer(){};
};

class CountMatrixCoverage : public CountMatrixTransformer
{
    public:
        CountMatrixCoverage(const unsigned long mean_read_length);
        virtual void transform(Matrix<float>& count_matrix, const std::vector<GeneMetadata>& genes_metadata);

    private:
        const float mean_read_length_;
};

inline CountMatrixCoverage::CountMatrixCoverage(const unsigned long mean_read_length)
    : mean_read_length_(static_cast<float>(mean_read_length))
{}

class CountMatrixNormalizer : public CountMatrixTransformer
{
    public:
        virtual void transform(Matrix<float>& count_matrix, const std::vector<GeneMetadata>& genes_metadata);
};

class CountMatrixDenormalizer : public CountMatrixTransformer
{
    public:
        CountMatrixDenormalizer(const double min_non_null);
        virtual void transform(Matrix<float>& count_matrix, const std::vector<GeneMetadata>& genes_metadata);

    private:
        const double min_non_null_;
};

inline CountMatrixDenormalizer::CountMatrixDenormalizer(const double min_non_null)
    : min_non_null_(min_non_null)
{}

class CountMatrixTransformerFactory
{
    public:
        static std::auto_ptr<CountMatrixTransformer> create_transformer(const std::string& transformation_type, const double min_non_null, const unsigned long mean_read_length);
    private:
        static const char COVERAGE_TRANSFORMATION_TYPE[];
        static const char NORMALIZE_TRANSFORMATION_TYPE[];
        static const char DENORMALIZE_TRANSFORMATION_TYPE[];
};

#endif //COUNT_MATRIX_TRANSFORMER_HH
