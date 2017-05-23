/*
 * Copyright 2016-2017 Enterome
 */ 

#include <iostream>
#include <new>
#include <cstdlib>
#include <stdexcept>
#include "Settings.hh"
#include "ExecutionDescription.hh"
#include "GenesLengthReader.hh"
#include "MatrixReader.hh"
#include "CountMatrixTransformer.hh"
#include "ExecutionDescriptionPrinter.hh"
#include "MatrixPrinter.hh"
#include "TimeProfiler.hh"

void no_more_memory()
{
    std::cerr << "error: memory allocation failed." << std::endl;
    std::exit (1);
}

int main(int argc, char *argv[])
{
    std::set_new_handler(no_more_memory);

    try
    {
        TimeProfiler time_profiler;
        time_profiler.start_new_timer("Total");

        const Settings& settings = Settings::parse(argc, argv);
        std::cout << settings << std::endl;

        const ExecutionDescription execution_description(argc, argv);

        std::auto_ptr<CountMatrixTransformer> count_matrix_transformer =
            CountMatrixTransformerFactory::create_transformer(settings.transformation.type, settings.transformation.min_non_null);

        std::cout << "Reading count matrix..." << std::endl;
        time_profiler.start_new_timer("Reading count matrix");
        MatrixReader<float> matrix_reader;
        Matrix<float> count_matrix = matrix_reader.read(settings.input.count_matrix_file);
        time_profiler.stop_last_timer();
        std::cout << "\rDone. Count matrix has " << count_matrix.ncol() << " samples and " << count_matrix.nrow() << " genes.\n" << std::endl;

        std::cout << "Reading genes length..." << std::endl;
        time_profiler.start_new_timer("Computing genes length");
        const std::vector<size_t>& genes_length = 
            GenesLengthReader::read(settings.input.genes_length_file, count_matrix);
        time_profiler.stop_last_timer();
        std::cout << "Done.\n" << std::endl;

        std::cout << "Transforming count matrix..." << std::endl;
        time_profiler.start_new_timer("Transforming count matrix");
        count_matrix_transformer->transform(count_matrix, genes_length);
        time_profiler.stop_last_timer();
        std::cout << "Done\n" << std::endl;

        std::cout << "Writing transformed count matrix..." << std::endl;
        time_profiler.start_new_timer("Writing transformed count matrix");
        ExecutionDescriptionPrinter::print(execution_description, settings.output.output_file);
        MatrixPrinter<float>::print(count_matrix, settings.output.output_file);
        time_profiler.stop_last_timer();
        std::cout << "\rDone         \n" << std::endl;

        time_profiler.stop_last_timer();
        std::cout << time_profiler << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(-1);
    }

    std::exit(0);
}
