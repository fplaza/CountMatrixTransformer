/*
 * Copyright 2016-2017 Enterome
 */ 

#include "CountMatrixTransformer.hh"
#include <limits>
#include <stdexcept>

void CountMatrixNormalizer::transform(Matrix<float>& count_matrix, const std::vector<size_t>& genes_length)
{
    //Step 1: Calculation of the copy number of each gene
    #pragma omp parallel for
    for (size_t curr_gene = 0; curr_gene < count_matrix.nrow(); ++curr_gene)
    {
        for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
            count_matrix(curr_gene, curr_sample) /= genes_length[curr_gene];
    }

    //Step 2: Calculation of the relative abundance of gene
    std::vector<double> samples(count_matrix.ncol());
    #pragma omp parallel
    {
        std::vector<double> samples_private(count_matrix.ncol());
        #pragma omp for nowait
        for (size_t curr_gene = 0; curr_gene < count_matrix.nrow(); ++curr_gene)
        {
            for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
                samples_private[curr_sample] += count_matrix(curr_gene, curr_sample);
        }

        for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
        {
            #pragma omp atomic
            samples[curr_sample] += samples_private[curr_sample];
        }

        #pragma omp barrier

        #pragma omp for
        for (size_t curr_gene = 0; curr_gene < count_matrix.nrow(); ++curr_gene)
        {
            for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)

                count_matrix(curr_gene, curr_sample) /= samples[curr_sample];
        }
    }
}

void CountMatrixDenormalizer::transform(Matrix<float>& count_matrix, const std::vector<size_t>& genes_length)
{
    std::vector<double> samples_coeffs(count_matrix.ncol(), std::numeric_limits<double>::max());

    #pragma omp parallel
    {
        std::vector<double> samples_coeffs_private(samples_coeffs);

        #pragma omp for nowait
        for (size_t curr_gene = 0; curr_gene < count_matrix.nrow(); ++curr_gene)
        {
            for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
            {
                const double count = count_matrix(curr_gene, curr_sample);
                if(count != 0.0)
                {
                    samples_coeffs_private[curr_sample] =
                        std::min(count*genes_length[curr_gene], samples_coeffs_private[curr_sample]);
                }
            }
        }

        #pragma omp critical
        for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
        {
            samples_coeffs[curr_sample] =
                std::min(samples_coeffs[curr_sample], samples_coeffs_private[curr_sample]);
        }

        #pragma omp barrier

        #pragma omp single
        {
            for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
            {
                samples_coeffs[curr_sample] = min_non_null_/samples_coeffs[curr_sample];
            }
        }

        // Step 2: denormalize
        #pragma omp for
        for (size_t curr_gene = 0; curr_gene < count_matrix.nrow(); ++curr_gene)
        {
            const size_t gene_coeff = genes_length[curr_gene];

            for (size_t curr_sample = 0; curr_sample < count_matrix.ncol(); ++curr_sample)
            {
                const double sample_coeff = samples_coeffs[curr_sample];

                count_matrix(curr_gene, curr_sample) =
                   static_cast<unsigned long>(count_matrix(curr_gene, curr_sample)*gene_coeff*sample_coeff + 0.5);
            }
        }
    }
}



std::auto_ptr<CountMatrixTransformer> CountMatrixTransformerFactory::create_transformer(const std::string& transformation_type, const double min_non_null)
{
    if (transformation_type.compare(NORMALIZE_TRANSFORMATION_TYPE) == 0)
    {
        return std::auto_ptr<CountMatrixTransformer>(new CountMatrixNormalizer());
    }
    else if (transformation_type.compare(DENORMALIZE_TRANSFORMATION_TYPE) == 0)
    {
        return std::auto_ptr<CountMatrixTransformer>(new CountMatrixDenormalizer(min_non_null));
    }
    else
    {
        throw (std::invalid_argument("error: '" +  transformation_type +  "' is not a valid transformation type."));
    }
}



const char CountMatrixTransformerFactory::NORMALIZE_TRANSFORMATION_TYPE[] = "normalize";
const char CountMatrixTransformerFactory::DENORMALIZE_TRANSFORMATION_TYPE[] = "denormalize";
